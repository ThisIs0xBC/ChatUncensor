#include "pch.h"
#include "ChatUncensor.h"

BAKKESMOD_PLUGIN(ChatUncensor, "Uncensor everything to bring toxicity back!", "v1.2", PLUGINTYPE_FREEPLAY)

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

	cvarManager->registerCvar("UnCensorChats", "1", "Uncensors chat messages.");

	gameWrapper->HookEventWithCaller<ActorWrapper>("Function TAGame.__GFxData_Chat_TA__AddChatMessage_0x1.__GFxData_Chat_TA__AddChatMessage_0x1",
												   [this](ActorWrapper caller, void* params, ...)
	{
		auto cUnCensorChats = _globalCvarManager->getCvar("UnCensorChats");
		if(!cUnCensorChats) return;
		if(!cUnCensorChats.getBoolValue()) return;
		pInternalSanitize* p = (pInternalSanitize*)params;
		if(!p) return;
		// store
		memcpy(&save, &p->Sanitized, sizeof(FString));
		// set 
		memcpy(&p->Sanitized, &p->_, sizeof(FString));
	});
	gameWrapper->HookEventWithCallerPost<ActorWrapper>("Function TAGame.__GFxData_Chat_TA__AddChatMessage_0x1.__GFxData_Chat_TA__AddChatMessage_0x1",
													   [this](ActorWrapper caller, void* params, ...)
	{
		auto cUnCensorChats = cvarManager->getCvar("UnCensorChats");
		if (!cUnCensorChats) return;
		if (!cUnCensorChats.getBoolValue()) return;
		pInternalSanitize* p = (pInternalSanitize*)params; 
		if(!p) return;

		if(!save.valid()) return;
		// not setting it back will result in a crash
		memcpy(&p->Sanitized, &save, sizeof(FString));
		// clear save
		memset(&save, 0, sizeof(FString));
	});
}
void ChatUncensor::RenderSettings()
{
	auto cUnCensorChats = cvarManager->getCvar("UnCensorChats");
	if (!cUnCensorChats)
	{
		ImGui::Text("Could not find cvar 'UnCensorChats'. please reload");
		return;
	}
	bool bUnCensorChats = cUnCensorChats.getBoolValue();
	if(ImGui::Checkbox("Enable", &bUnCensorChats))
		cUnCensorChats.setValue(bUnCensorChats);
}
