# ShootingGame

## Project Overview
ShootingGame is a 2D vertical shooting game developed using C++ and the Windows API (WinAPI). It features an actor-component architecture, scene management, and several specialized managers for handling resources, input, collisions, and more.

### Main Technologies
- **Language:** C++20
- **Graphics:** Windows GDI (WinAPI) with Double Buffering
- **Data:** `nlohmann/json` for JSON parsing
- **Utilities:** `magic_enum` for enum-to-string conversions
- **Architecture:** Actor-Component system, Singleton managers, Scene-based state management

### Key Architecture Components
- **Game Engine (`Game` class):** A singleton that initializes the window, manages the main loop, and handles scene transitions.
- **Actor-Component System:** Game objects are `Actor` instances that can have various components like `Sprite`, `Texture`, and `ColliderCircle`.
- **Managers:**
  - `ResourceManager`: Loads and manages bitmaps and fonts.
  - `TimeManager`: Handles delta time and FPS.
  - `InputManager`: Processes keyboard and mouse input.
  - `CollisionManager`: Manages circle-based collision detection.
  - `DataManager`: Loads game data from JSON files.
  - `UIManager`: Handles UI rendering and logic.
- **Object Pooling:** Used for managing bullets (`Bullet`) to optimize performance and reduce memory fragmentation.
- **Collision Detection:** Implemented using a grid-based spatial partitioning system (implied by `Cell` and `GridInfo` in `Defines.h`) and circle colliders.

## Building and Running

### Prerequisites
- Windows OS
- Visual Studio (with C++ development workload)

### Build Instructions
1. Open `ShootingGame.sln` in Visual Studio.
2. Set the build configuration to `Debug` or `Release` and the platform to `x64`.
3. Build the solution (Ctrl+Shift+B).

### Running the Game
- Run the executable from Visual Studio (F5) or directly from `x64/Debug/ShootingGame.exe` or `x64/Release/ShootingGame.exe`.
- **Note:** The game expects a `Resources` directory at `../Resources/` relative to the executable path. Ensure the `Resources` folder is correctly placed.

## Development Conventions

### Coding Style
- **Naming:** 
  - Classes: PascalCase (e.g., `GameScene`, `Actor`)
  - Methods: PascalCase (e.g., `Update`, `Render`, `GetPos`)
  - Member Variables: Prefixed with an underscore (e.g., `_pos`, `_components`)
  - Constants/Globals: Prefixed with `G` (e.g., `GWinSizeX`)
- **Memory Management:**
  - Uses the `SAFE_DELETE(p)` macro for safe deletion.
  - Memory leak detection is enabled in Debug mode via `_CrtSetDbgFlag`.
  - Prefer using `ObjectPool` for frequently created/destroyed objects.

### Scene Management
- Scenes should inherit from the `Scene` base class.
- The `Game` class manages the current scene and handles initialization and cleanup during transitions.

### Resource Management
- All bitmaps and fonts should be loaded through `ResourceManager`.
- Resource paths are relative to the `Resources` directory.

### Debugging
- Press `F1` in-game to toggle collider debug drawing.
- Press `F2` in-game to toggle grid debug drawing.
