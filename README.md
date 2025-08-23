Chat Uncensor (BakkesMod Plugin)

Removes the new in-game chat censoring in Rocket League so you can see what was actually sent. Optionally, you can still show the original censored text alongside the uncensored version.

⚠️ Use responsibly. This plugin does not encourage harassment or abusive language. It only affects what you see locally and does not change what other players receive.

Features

Uncensors incoming Rocket League chat messages in your client

Optional dual-display: show uncensored and censored versions together

Lightweight, automatic on load

Integrates with BakkesMod’s plugin list

Quick Install

Download the DLL
Grab the ChatUncensor.dll from the Latest Release on the GitHub Releases page.

Copy to plugins folder
Place the DLL here:

%appdata%\bakkesmod\bakkesmod\plugins


Register the plugin
Add this line on a new line in:

%appdata%\bakkesmod\bakkesmod\cfg\plugins.cfg

plugin load chatuncensor


Launch BakkesMod/Rocket League
The plugin should appear in the BakkesMod Plugins list.

Usage

Once loaded, the plugin runs automatically. Incoming chat will be shown uncensored.

Optional: Show censored text alongside uncensored

If you prefer to see both versions for context, enable the dual-display option you can toggle it via the plugin’s settings UI

Troubleshooting

Doesn’t show in Plugins list

Recheck the DLL path is exactly:

%appdata%\bakkesmod\bakkesmod\plugins\ChatUncensor.dll


Confirm plugins.cfg has the exact line plugin load chatuncensor on its own line.

Restart BakkesMod and Rocket League.

No change to chat

Make sure the plugin is enabled in the plugins list.

Errors on load

Ensure you’re on a supported BakkesMod build and a current Rocket League version.

Redownload the DLL from Latest Release in case of a corrupt file.

Building From Source

Clone the repository

Open the solution in Visual Studio (vcpkg/BakkesMod SDK set up as needed)

Build Release | x64

Copy the resulting DLL into:

%appdata%\bakkesmod\bakkesmod\plugins


Add plugin load chatuncensor to plugins.cfg if you haven’t already

FAQ

Does this change what other players see?
No. It only affects your local client’s display.

Will this get me banned?
This plugin simply changes how messages render locally. It doesn’t affect gameplay or modify network traffic. Therefore it will not be the cause of a ban.

Can I turn it off quickly?
Yes. In the plugin list, disable it

Support & Feedback

Open an Issue on GitHub with steps to reproduce any problem

Include your BakkesMod log if possible (%appdata%\bakkesmod\bakkesmod\logs)
