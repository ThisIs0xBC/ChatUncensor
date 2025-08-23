#include "pch.h"
#include "ChatUncensor.h"

BAKKESMOD_PLUGIN(ChatUncensor, "Uncensor everything to bring toxicity back!", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

/*

	Updated and optimized by SoulDaMeep

	New Method: change the internal sanitize function data to use
	 | the non sanitized string passed into it
	Cons:
		Might work for names...but honestly a W????
	Pros:
		Like psyonix implemented a setting for it instead of some
		weird workaround


	Old Method: Use LogToChatbox to simulate the message being set
	 |  before it was censored.
	Cons:
		the team colors were off
		timestamps
		stange characters
	Pros:
		it did...'work'...?


*/
void ChatUncensor::onLoad()
{
	_globalCvarManager = cvarManager;

	cvarManager->registerCvar("uncensor_chats", "1", "Uncensors chat messages.");
	cvarManager->registerCvar("uncensor_chats_highlight_censored_words", "1", "Highlights which words were censored in the uncensored message.");

	gameWrapper->HookEventWithCaller<ActorWrapper>(
		"Function TAGame.__GFxData_Chat_TA__AddChatMessage_0x1.__GFxData_Chat_TA__AddChatMessage_0x1",
		[this](ActorWrapper caller, void* params, ...)
	{
		auto cUncensorChats = cvarManager->getCvar("uncensor_chats");
		if (!cUncensorChats || !cUncensorChats.getBoolValue()) return;

		pInternalSanitize* p = (pInternalSanitize*)params;
		if (!p) return;

		// store
		memcpy(&save, &p->Sanitized, sizeof(FString));

		auto cHighlightCensoredWords = cvarManager->getCvar("uncensor_chats_highlight_censored_words");
		if (cHighlightCensoredWords && cHighlightCensoredWords.getBoolValue()) {
			std::string highlightedMsg = highlightCensoredWords(p->_, p->Sanitized);
			if (!highlightedMsg.empty()) {
				gameWrapper->Execute([this, highlightedMsg](GameWrapper* gw) {
					gameWrapper->LogToChatbox(highlightedMsg, "Uncensored");
				});
			}
		}

		// set 
		memcpy(&p->Sanitized, &p->_, sizeof(FString));
	});

	gameWrapper->HookEventWithCallerPost<ActorWrapper>(
		"Function TAGame.__GFxData_Chat_TA__AddChatMessage_0x1.__GFxData_Chat_TA__AddChatMessage_0x1",
		[this](ActorWrapper caller, void* params, ...)
	{
		auto cUncensorChats = cvarManager->getCvar("uncensor_chats");
		if (!cUncensorChats || !cUncensorChats.getBoolValue()) return;

		pInternalSanitize* p = (pInternalSanitize*)params;
		if (!p) return;

		if (!save.valid()) return;
		// not setting it back will result in a crash
		memcpy(&p->Sanitized, &save, sizeof(FString));
		// clear save
		memset(&save, 0, sizeof(FString));
	});
}

std::string ChatUncensor::highlightCensoredWords(FString origMsg, FString censoredMsg) {

	// Convert FString (wide) to std::wstring for easier handling
	std::wstring origW(origMsg.getData(), origMsg.length());
	std::wstring censW(censoredMsg.getData(), censoredMsg.length());

	// If there are no censored characters, return empty
	if (censW.find(L'*') == std::wstring::npos) {
		return "";
	}

	std::wstring result;
	bool inCensor = false;
	std::wstring buffer;

	for (size_t i = 0; i < censW.size() && i < origW.size(); ++i) {
		if (censW[i] == L'*') {
			// Start of a censored block
			if (!inCensor) {
				inCensor = true;
				buffer.clear();
			}
			buffer.push_back(origW[i]); // Get original char
		}
		else {
			if (inCensor) {
				// Censorred block ended, wrap with brackets
				result.append(L"[");
				result.append(buffer);
				result.append(L"]");
				buffer.clear();
				inCensor = false;
			}
			result.push_back(origW[i]); // Uncensored character
		}
	}

	// If the message ended while still inside a censored block, wrap it
	if (inCensor) {
		result.append(L"[");
		result.append(buffer);
		result.append(L"]");
	}

	// Convert back to UTF8 std::string
	std::string out;
	out.resize(result.size() * 4); // worst case UTF-8 expansion
	int len = WideCharToMultiByte(CP_UTF8, 0, result.c_str(), (int)result.size(),
		out.data(), (int)out.size(), nullptr, nullptr);
	out.resize(len);
	return out;
}

void ChatUncensor::onUnload() {
	gameWrapper->UnhookEvent("Function TAGame.__GFxData_Chat_TA__AddChatMessage_0x1.__GFxData_Chat_TA__AddChatMessage_0x1");
}

void Spacing(int amount)
{
	for (int i = 0; i < amount; i++) {
		ImGui::Spacing();
	}
}

void ChatUncensor::RenderSettings()
{
	auto cUncensorChats = cvarManager->getCvar("uncensor_chats");
	auto cHighlightCensoredWords = cvarManager->getCvar("uncensor_chats_highlight_censored_words");
	if (!cUncensorChats || !cHighlightCensoredWords)
	{
		ImGui::Text("Could not find cvar 'uncensor_chats' or 'uncensor_chats_highlight_censored_words'! Please reload the plugin.");
		return;
	}

	if (ImGui::BeginChild("Header", ImVec2(0, 100.0f), false))
	{
		Spacing(4);

		ImGui::TextColored(ImVec4(1, 0, 1, 1), "Plugin made by 0xBC (Bangers n' Nyash)");
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Optimised by SoulDaMeep");

		Spacing(3);

		ImGui::Text(pretty_plugin_version);
		ImGui::Separator();
	}
	ImGui::EndChild();

	bool bUncensorChats = cUncensorChats.getBoolValue();
	if (ImGui::Checkbox("Uncensor Chats", &bUncensorChats))
		cUncensorChats.setValue(bUncensorChats);

	bool bHighlightCensoredWords = cHighlightCensoredWords.getBoolValue();
	if (ImGui::Checkbox("Highlight Censored Words", &bHighlightCensoredWords))
		cHighlightCensoredWords.setValue(bHighlightCensoredWords);
}
