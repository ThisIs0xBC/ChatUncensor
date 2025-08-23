# Chat Uncensor (BakkesMod Plugin)

Removes the new in-game chat censoring in Rocket League so you can see what was actually sent. Optionally, you can still show the original censored text alongside the uncensored version.

> WARNING: Use responsibly. This plugin does **not** encourage harassment or abusive language. It only affects what **you** see locally and does not change what other players receive.

---

## Features

- Uncensors incoming Rocket League chat messages in your client
- Optional dual-display: show uncensored and censored versions together
- Lightweight, automatic on load
- Integrates with BakkesMod’s plugin list

---

## Quick Install

1. **Download the DLL**  
   Grab the `ChatUncensor.dll` from the Latest Release on the GitHub Releases page.

2. **Copy to plugins folder**  
   Place the DLL here:
   ```
   %appdata%\bakkesmod\bakkesmod\plugins
   ```
4. **Register the plugin**  
   Add this line on a new line in:
   ```
   %appdata%\bakkesmod\bakkesmod\cfg\plugins.cfg
   ```
   ```
   plugin load chatuncensor
   ```
6. **Launch BakkesMod/Rocket League**  
   The plugin should appear in the BakkesMod Plugins list.

---

## Usage

Once loaded, the plugin runs automatically and incoming chat will be shown uncensored.

### Optional: Show censored text alongside uncensored
If you prefer to see both versions for context, enable the dual-display option.

- Toggle via the plugin’s settings UI, or
- Press F6 to open the Bakkesmod console, and type
  `
  toggle uncensor_chats_remove_original_message 0
  `

## Troubleshooting

- **Doesn’t show in Plugins list**
  - Recheck the DLL path is exactly:
    ```
    %appdata%\bakkesmod\bakkesmod\plugins\ChatUncensor.dll
    ```
  - Confirm `plugins.cfg` has the exact line `plugin load chatuncensor` on its own line.
  - Restart BakkesMod and Rocket League.

- **No change to chat**
  - Make sure the plugin is enabled in the plugins list.
  - If you use other custom chat plugins such as BetterChat etc, test with only Chat Uncensor enabled to rule out conflicts.

- **Errors on load**
  - Ensure you’re on a supported BakkesMod build and a current Rocket League version.
  - Redownload the DLL from Latest Release in case of a corrupt file.

---

## Building From Source

1. Clone the repository
2. Open the solution in Visual Studio (vcpkg/BakkesMod SDK set up as needed)
3. Build `Release | x64`
4. Copy the resulting DLL into:
   ```
   %appdata%\bakkesmod\bakkesmod\plugins
   ```
6. Add `plugin load chatuncensor` to `plugins.cfg` if you haven’t already

---

## FAQ

**Does this change what other players see?**  
No. It only affects your local client’s display.

**Will this get me banned?**  
This plugin simply changes how messages render locally. It doesn’t affect gameplay or modify network traffic. As such, it is completely safe to use, as is any other Bakkesmod plugin.

**Can I turn it off quickly?**  
Yes. In the plugin list, disable it, or run:  
`toggle uncensor_chats 0`

---

## Support & Feedback

- Open an Issue on GitHub with steps to reproduce any problem

---

## License

MIT (see `LICENSE` in this repository).
