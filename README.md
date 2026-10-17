# DeadZone

Unreal Engine 5.7 project. This repository contains **source code and configuration only** — game assets under `Content/` are **not** included (see [Assets](#assets)).

## Requirements

- Unreal Engine **5.7**
- Enabled plugins (listed in `DeadZone.uproject`):
  - GameplayAbilities
  - CommonUI
  - ModelingToolsEditorMode (editor only)
  - ThumbnailExportManager (editor only)

## Repository Contents

| Path | Description |
| --- | --- |
| `Source/` | C++ game module (`DeadZone`) — 57 headers, 56 sources, 1 build script |
| `Config/` | Project configuration (engine/editor/gameplay INI) |
| `DeadZone.uproject` | Project descriptor (engine association, plugins) |
| `DeadZone.sln` | Visual Studio solution |
| `DeadZone.Target.cs` / `DeadZoneEditor.Target.cs` | Build targets |

## Setup

1. Clone this repository.
2. Open `DeadZone.uproject` with Unreal Engine 5.7 — it will prompt to rebuild the `DeadZone` C++ module, accept.
3. Install the required marketplace asset packs into `Content/` (the project will report missing references until they are in place).

## Assets

All art/audio/blueprint content lives in `Content/` and is distributed via the Unreal Engine Marketplace, not via this repository. Packs used by the project include (Synty Studios and third-party GUI/animation kits):

- POLYGON Military / Apocalypse / City Zombies / Heist / Battle Royale
- Zombie character & animation kits
- Horror grunge GUI pack

Check the licensing terms of each pack before redistributing any derived content.

## License

Source code in this repository is provided as-is. Marketplace assets are governed by their respective Unreal Engine Marketplace licenses and are not covered by this repository.
