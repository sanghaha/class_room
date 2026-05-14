# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

Open `BounceBall.sln` in Visual Studio 2022 and build via IDE (preferred). Alternatively:

```
msbuild BounceBall.sln /p:Configuration=Debug /p:Platform=x64
msbuild BounceBall.sln /p:Configuration=Release /p:Platform=x64
```

Output: `x64\Debug\BounceBall.exe` or `x64\Release\BounceBall.exe`.

## Project Configuration

- **C++ Standard**: C++20 (`stdcpp20`)
- **Platform Toolset**: MSVC v143 (Visual Studio 2022), targeting x64
- **Key Libraries**: `gdiplus.lib`, `dsound.lib`, `winmm.lib`, `msimg32.lib`
- **Precompiled Headers**: `pch.h` (included in every `.cpp`)
- **External Headers**: `nlohmann/json` and `magic_enum` as source-level includes in `BounceBall/`

## Architecture Overview

BounceBall_HDC is a Windows GDI+ brick-breaker game using an Actor-Component-Scene architecture with singleton managers.

### Core Hierarchy

```
Game (Singleton)
  └─ Scene (abstract world container)
       ├─ Actors (entities)
       │    ├─ Ball, Block, Star, Effect, Background
       │    └─ Components attached per actor:
       │         SpriteRenderer, ImageRenderer, RectCollider
       └─ UIManager (buttons, images per scene)

Singleton Managers (global services):
  TimeManager, InputManager, ResourceManager, DataManager, SoundManager
```

### Key Design Decisions

**Component Composition**
`Actor` is the base entity. Behavior comes from attached components (`GetComponent<T>()`). Components implement `InitComponent()`, `UpdateComponent(dt)`, `RenderComponent(hdc, pos)`.

**Deferred Actor Lifecycle**
`ReserveAdd()` / `ReserveRemove()` queue changes; actual mutation happens after iteration in `Scene::Update()`. Prevents invalidation during traversal. `AddPostUpdateAction()` schedules deferred callbacks.

**Grid Spatial Partitioning**
`Scene` maintains a 2D grid (cell = 64px = `BLOCK_SIZE`). `UpdateGrid(actor, prevPos, nextPos)` tracks actors spatially. `GameScene::CheckCollision()` queries only nearby cells.

**Render Layer System**
Actors return `GetRenderLayer()` (Background=0, Object=1, Ball=2, Effect=3). Scene maintains per-layer render lists in `_renderList[RenderLayer]` for correct z-order without sorting.

**Double Buffering**
`Game` maintains `_hdcBack` / `_bmpBack`. All rendering targets `_hdcBack`; `BitBlt()` copies to screen at end of frame.

### Game Loop (`BounceBall.cpp`)

Targets 120 FPS via `QueryPerformanceCounter`. Each iteration: `Game::Update()` (all managers + current scene) → `Game::Render()` (draw to back buffer → BitBlt).

### Scenes

| Scene | Purpose |
|-------|---------|
| `LobbyScene` | Title screen; transitions to GameScene or EditorScene |
| `GameScene` | Active gameplay; loads `.stage` files, runs collision/physics, spawns Effects |
| `EditorScene` | Level editor; place/remove Block/Star/Ball, save/load `.stage` files |
| `EmptyScene` | Startup placeholder |

### Collision System (`GameScene`)

- **`CheckCollision`**: Line-segment casting (movement vector → smallest t), continuous detection
- **`CheckCollision_ClosestPoint`**: Circle vs AABB discrete check, returns contact normal
- Bitmasks in `Defines.h`: `COLLISION_BIT_MASK_BLOCK` (hard), `COLLISION_BIT_MASK_OVERLAP` (trigger/stars)
- Helper math: `LineIntersectsAABB()`, `CheckCircleAABB()`, `IntersectSegmentRect()` in `Defines.h`

### Ball Physics (`Ball.h/cpp`)

Newtonian simulation: velocity + acceleration each frame. Key parameters in `Ball.h`:
- `gravityVec` (0, 980), `maxFallSpeed` (1200)
- `xFriction`, `xFrictionExp` (horizontal dampening)
- `moveForce/mass` (input force scaling), `bounceDampX` (collision dampening)
- Input: Left/Right arrows = horizontal movement; SpaceBar = jump when grounded

### Resource Management

`ResourceManager::LoadTexture(key, path, countX, countY, transparent)` loads BMP sprite sheets. Sprite names (e.g., `"Ball_0"`) map to frame indices + animation duration. Sounds loaded via `LoadSound(key, path)` (DirectSound WAV). All assets cached by string key.

### Input System

`InputManager` tracks key states: None, Press (held), Down (just pressed), Up (just released). Query with `GetButtonPressed/Down/Up(KeyType)`. Mouse position via `GetMousePos()`. F3 toggles frame-step debug mode.

## Code Conventions

- Private members prefixed with `_` (e.g., `_pos`, `_actors`)
- Custom int types: `int8`, `int16`, `int32`, `int64`, `uint8`, etc. (defined in `pch.h`)
- `Vector` struct (2D) with operator overloads (+, -, *, dot, cross, normalize, rotate) in `Defines.h`
- `SAFE_DELETE()` / `SAFE_RELEASE()` macros for cleanup
- Korean comments are used throughout — preserve them when editing

## Resources

- `Resources/Image/` — BMP sprite sheets (frame grids, e.g. `2x4 = 8 frames`)
- `Resources/Sound/` — WAV files
- `Resources/Stage/` — `.stage` text files (loaded by `StageLoader`)
- `Resources/Font/` — TTF fonts (MaplestoryBold, MaplestoryLight)
