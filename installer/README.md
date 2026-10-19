# Swofty Launcher Installer

A small window that installs the latest Swofty Launcher release and opens it.

1. Download [SwoftyLauncherInstaller.zip](https://github.com/Swofty-Developments/SwoftyLauncher/raw/main/installer/SwoftyLauncherInstaller.zip) and extract it.
2. Run `SwoftyLauncherInstaller.exe` on Windows, or `SwoftyLauncherInstaller` on Linux and Steam Deck.
3. Choose a folder and select **Install**.

The installer downloads the latest release from GitHub, checks it against the release's `SHA256SUMS`, extracts it into the folder, adds a Start menu or app menu entry and, if ticked, a desktop shortcut, then opens the launcher. Running it again on the same folder updates that install. After the first install the launcher updates itself.

Default folders:

| Platform | Folder |
|---|---|
| Windows | `%LOCALAPPDATA%\Programs\SwoftyLauncher` |
| Linux | `~/SwoftyLauncher` |

## Building

```bash
cargo build --release -p swofty-launcher-installer
```

CI builds both executables on every push (`Installer` job, artifacts `installer-Linux` and `installer-Windows`). `SwoftyLauncherInstaller.zip` holds those two executables, renamed to `SwoftyLauncherInstaller` and `SwoftyLauncherInstaller.exe`.
