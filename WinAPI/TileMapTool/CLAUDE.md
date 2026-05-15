# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

Open `TileMapTool.sln` in Visual Studio 2022 and build with **Ctrl+Shift+B**, or use the Developer Command Prompt:

```
msbuild TileMapTool.sln /p:Configuration=Debug /p:Platform=x64
```

Output: `x64\Debug\TileMapTool.exe`

The project requires the C++20 standard (uses `std::format`, `std::filesystem`). No external dependencies beyond Win32 and `msimg32.lib` (linked via pragma in `pch.h`).

## Architecture

Two-window Win32 application with a game-loop-style message pump (`PeekMessage` + no-message branch for Update/Render).

**Entry point & windowing** — `TileMapTool.cpp`  
Registers two window classes, creates two `HWND`s (`gMainWnd`, `gSubWnd`), then runs the loop calling `ToolManager → Update → Render` each idle tick.

**ToolManager** (Singleton) — `ToolManager.h/cpp`  
Central coordinator. Owns a `TileMapMain*` and a `TileMapSub*`, delegates Update/Render to both, and exposes `GetSelectedTileIndex()` so Main can read Sub's selection.

**TileMapMain** — `TileMapMain.h/cpp`  
Handles the editing canvas (20×15 grid). Stores 4 independent `TileLayer` arrays of tile indices (-1 = empty). Paints tiles from the tilemap BMP via `TransparentBlt`. Keyboard hotkeys:

| Key | Action |
|-----|--------|
| Left-drag | Paint selected tile onto current layer |
| Right-click | Erase tile |
| S | Save (`.tilemap` file dialog) |
| L | Load (`.tilemap` file dialog) |
| Q / E | Increase / decrease selected layer |
| F1 | Toggle showing only the current layer |

**TileMapSub** — `TileMapSub.h/cpp`  
Shows the full tile palette BMP. Left-click selects a tile index. Draws a red rectangle around the selected tile.

**InputManager** (Singleton) — `InputManager.h/cpp`  
Polls `GetAsyncKeyState` each frame. Tracks per-key state transitions (`None → Down → Press → Up → None`). Maintains separate mouse positions for main and sub windows via `GetCursorPos` + `ScreenToClient`.

**Defines** — `Defines.h/cpp`  
All grid/tile constants and the `TileInfo` structs that define which tile-sheet rectangles are valid for each layer:

```
TileSize = 64px (rendered)     OriginTileSize = 64px (source)
GridWidth = 20, GridHeight = 15         (editing canvas)
TileMapWidth = 16, TileMapHeight = 8    (tile palette sheet)
```

Layer → valid tile regions: Layer 0 = Water, Layer 1 = Rock, Layer 2 = Ground/Bridge, Layer 3 = GoldMine.

**Singleton** — `Singleton.h`  
CRTP template. Use `T::GetInstance()` / `T::DestroyInstance()`.

## Tilemap File Format (`.tilemap`)

```
GridWidth,GridHeight,TileMapWidth,TileMapHeight
layerIndex:tileCount
x,y,tileIndex
...   (repeated tileCount times)
(repeated per layer)
```

## Resources

BMP files live in `Resources/`. The transparent color is magenta `RGB(255, 0, 255)`.
