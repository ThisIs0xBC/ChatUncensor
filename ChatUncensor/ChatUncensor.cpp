#include "pch.h"
#include "ChatUncensor.h"
#include <utils.h>

BAKKESMOD_PLUGIN(ChatUncensor, "Uncensor chats to bring toxicity back!", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

// Simple key: player name + channel
struct SenderKey {
	std::string name;
	uint8_t channel;

	bool operator==(const SenderKey& o) const {
		return channel == o.channel && name == o.name;
	}
};
struct SenderKeyHash {
	size_t operator()(const SenderKey& k) const {
		return std::hash<std::string>{}(k.name) ^ (size_t(k.channel) << 1);
	}
};

// Global cache of last uncensored text per sender
static std::unordered_map<SenderKey, std::string, SenderKeyHash> g_lastUncensored;

// Convert string to lower case (not really needed but whatever)
static inline std::string norm(const std::string& s) {
	std::string out = s;
	for (auto& c : out) c = (char)std::tolower((unsigned char)c);
	return out;
}

void ChatUncensor::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("uncensor_chats", "1", "Uncensors chat messages.");
	cvarManager->registerCvar("uncensor_chats_remove_original_message", "1", "Removes the original, censored message, and only displays the uncensored one.");
	static auto uncensorChatsCvar = _globalCvarManager->getCvar("uncensor_chats");
	static auto removeOriginalMessageCvar = _globalCvarManager->getCvar("uncensor_chats_remove_original_message");

	this->HookChat();
	this->HookGameStart();

}

void ChatUncensor::onUnload() {
	this->ReleaseHooks();
}

void ChatUncensor::ReleaseHooks() {
	gameWrapper->UnhookEvent("Function TAGame.GameEvent_Soccar_TA.InitGame");
	gameWrapper->UnhookEvent("Function TAGame.HUDBase_TA.OnChatMessage");
	gameWrapper->UnhookEvent("Function TAGame.GFxData_Chat_TA.OnChatMessage");
}

void ChatUncensor::HookGameStart() {
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.InitGame", [this](std::string eventName) {
		this->FixQueue.clear();
	});
}

void ChatUncensor::HookChat() {
	// https://github.com/JulienML/BetterChat/blob/fd0650ae30c12c11c70302045cfd9d4b6e181759/BetterChat.cpp#L509
	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.HUDBase_TA.OnChatMessage", [this](ActorWrapper Caller, void* params, ...) {
		if (params) {
			ChatMessage1* message = (ChatMessage1*)params;

			if (!message)
				return;

			// Prevents crashing when joining a party and just other general crashes
			if (message->PlayerName == nullptr) return;
			if (message->Message == nullptr) return;
			if (message->bPreset) return;

			std::string playerName = norm(wToString(message->PlayerName));
			if (playerName.empty()) return;

			SenderKey key{ playerName, message->ChatChannel };
			g_lastUncensored[key] = wToString(message->Message);
		}
	});

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.GFxData_Chat_TA.OnChatMessage", [this](ActorWrapper Caller, void* params, std::string eventName) {
		FGFxChatMessage* message = (FGFxChatMessage*)params;

		if (!message)
			return;

		// Stole this idea from https://github.com/0xleft/trnslt cheers pal
		// We basically need to fix the team/chat channel of the msgs from LogToChatbox as it will trigger this hook again when we send an uncensored message
		auto fixIterator = std::find_if(this->FixQueue.begin(), this->FixQueue.end(), [message](LogMessage& logMessage) {
			return logMessage.Message == message->Message.ToString() && logMessage.PlayerName == message->PlayerName.ToString();
		});

		if (fixIterator != this->FixQueue.end()) {

			// If the uncensored msg we are displaying via LogToChatbox is from us, then make it white like normal text chat from ourselves
			// We could change the player name in here to make this more reliable, as if a person called "YOU" sends a censored msg
			// this code will make it look like you sent it yourself, not the other guy,
			// but that involves generating FString objects in this hook (as PlayerName in FGFxChatMessage is an FString)
			// and that will create a very slow memory leak, so stuck with this method instead
			if (fixIterator->PlayerName.ends_with("[UC] YOU"))
				message->Team = -1;
			else
				message->Team = fixIterator->Team; // If we didn't send the (un)censored msg, just make it the correct team colour

			message->ChatChannel = fixIterator->ChatChannel; // Ensure its sent to global/team/party chat correctly

			this->FixQueue.erase(fixIterator);
		}

		std::string playerName = norm(message->PlayerName.ToString());
		SenderKey key{ playerName, message->ChatChannel };

		// Only bother if the displayed text looks censored
		std::string censored = message->Message.ToString();
		if (censored.find('*') == std::string::npos) 
			return;

		auto it = g_lastUncensored.find(key);
		if (it == g_lastUncensored.end()) return;

		const std::string& uncensored = it->second;
		if (!uncensored.empty() && uncensored != censored) {

			LogMessage fakeMsg{
				uncensored,
				message->ChatChannel,
				message->PlayerName.ToString(),
				message->Team,
				message->TimeStamp.ToString()
			};

			if (_globalCvarManager->getCvar("uncensor_chats_remove_original_message").getBoolValue()) {
				RemoveOriginalMessage(message);
			}

			RemoveCensor(fakeMsg);
		}

		// Consume to avoid accidental reuse
		g_lastUncensored.erase(it);
	});
}

void ChatUncensor::RemoveCensor(LogMessage message) {

	gameWrapper->Execute([this, message](GameWrapper* gw) {
		LogMessage logMessage = message;
		if (logMessage.PlayerName == gameWrapper->GetPlayerName().ToString()) {
			logMessage.PlayerName = std::format("[{}] [UC] YOU", logMessage.TimeStamp);
		}
		else {
			logMessage.PlayerName = std::format("[{}] [UC] {}", logMessage.TimeStamp, logMessage.PlayerName);
		}
		
		this->FixQueue.push_back(logMessage);
		gameWrapper->LogToChatbox(logMessage.Message, logMessage.PlayerName);
	});
}

// Set the array count of all the FStrings to 0 to effectively delete them without actually deleting them (no heap allocations this way)
void ChatUncensor::RemoveOriginalMessage(FGFxChatMessage* origMsg) {
	origMsg->TimeStamp.Clear();
	origMsg->PlayerName.Clear();
	origMsg->Message.Clear();
	origMsg->ChatChannel = 0;
}