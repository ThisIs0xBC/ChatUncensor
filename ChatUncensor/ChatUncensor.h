#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include <utils.h>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr auto pretty_plugin_version = "v" stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH);

extern std::string toLower(std::string str);

class ChatUncensor: public BakkesMod::Plugin::BakkesModPlugin
	,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
	//,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	void onLoad() override;
	void onUnload() override; // Uncomment and implement if you need a unload method

public:
	// Stole this idea from https://github.com/0xleft/trnslt cheers pal
	std::vector<LogMessage> FixQueue;

	void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
	//void RenderWindow() override; // Uncomment if you want to render your own plugin window

	void ReleaseHooks();
	void HookGameStart();
	void HookChat();
	void RemoveCensor(LogMessage message);
	void RemoveOriginalMessage(FGFxChatMessage* message);
	
};
