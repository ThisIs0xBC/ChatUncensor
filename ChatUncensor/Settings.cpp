#include "pch.h"
#include "ChatUncensor.h"

void Spacing(int amount)
{
	for (int i = 0; i < amount; i++) {
		ImGui::Spacing();
	}
}

void ChatUncensor::RenderSettings() {
	static auto uncensorChatsCvar = _globalCvarManager->getCvar("uncensor_chats");
	static auto removeOriginalMessageCvar = _globalCvarManager->getCvar("uncensor_chats_remove_original_message");
	bool uncensorChats = uncensorChatsCvar.getBoolValue();
	bool removeOriginalMessage = removeOriginalMessageCvar.getBoolValue();

	if (ImGui::BeginChild("Header", ImVec2(0, 80.0f), false))
	{
		Spacing(4);

		ImGui::TextColored(ImVec4(1, 0, 1, 1), "Plugin made by 0xBC (Bangers n' Nyash)");

		Spacing(3);

		ImGui::Text(pretty_plugin_version);
		ImGui::Separator();
	}
	ImGui::EndChild();

	if (ImGui::Checkbox("Uncensor Chats", &uncensorChats))
	{
		uncensorChatsCvar.setValue(uncensorChats);
	}

	if (ImGui::Checkbox("Remove Original Message", &removeOriginalMessage))
	{
		removeOriginalMessageCvar.setValue(removeOriginalMessage);
	}

	Spacing(2);

}
