#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include <Windows.h>

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);

constexpr auto pretty_plugin_version = "v" stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH);

struct FString
{
private:
	// variables
	wchar_t* data;
	int32_t count;
	int32_t max;
public:
	// constructors
	FString() = default;
public:	
	// functions
	bool valid() { return data != nullptr && count > 0 && max > 0;}
};

struct pInternalSanitize
{
	FString _; // original
	FString Sanitized; // sanitized
};

class ChatUncensor: 
	public BakkesMod::Plugin::BakkesModPlugin,
	public SettingsWindowBase
{
public:
	// overrides
	void onLoad()		  override;
	void RenderSettings() override; 
public:
	// variables
	FString save;
};
