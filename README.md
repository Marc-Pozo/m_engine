# M-Engine

A learning-focused 3D game engine and editor built in C++ with OpenGL. The project supports development on Windows and Ubuntu, using CMake for reproducible builds and Make for convenient development commands.

> **Current status:** Bootstrap stage. The initial goal is a working OpenGL application, followed by a first 3D renderer and progressively more engine and editor systems.

## Table of Contents

- [Goals](#goals)
- [Technology Stack](#technology-stack)
- [Prerequisites](#prerequisites)
- [Windows Setup](#windows-setup)
- [Ubuntu Setup](#ubuntu-setup)
- [Build, Run, and Hot Reload](#build-run-and-hot-reload)
- [Project Structure](#project-structure)
- [Git Workflow](#git-workflow)
- [Commit Conventions](#commit-conventions)
- [Roadmap](#roadmap)

## Goals

M-Engine is a solo-developer learning and portfolio project. Its goals are to:

- Build a 3D rendering pipeline using modern OpenGL.
- Develop a clear, modular C++ engine architecture.
- Create an editor for inspecting and working with scenes.
- Use reliable build, testing, and version-control practices.
- Maintain a consistent development workflow across Windows and Linux.

## Technology Stack

| Area | Technology |
|---|---|
| Language | C++20 |
| Graphics API | OpenGL |
| Window and input | GLFW |
| OpenGL function loading | GLAD 2 |
| Mathematics | GLM |
| Shaders | GLSL |
| Build system | CMake |
| Build presets | CMake Presets |
| Development commands | GNU Make |
| File watching and hot reload | watchexec |
| Windows compiler | MSVC |
| Linux compiler | GCC |
| Version control | Git |

The application requires a graphics driver that supports the OpenGL version requested by the application. The application reports the OpenGL version supplied by the driver at startup.

## Prerequisites

### Windows

Install the following tools:

- [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with **Desktop development with C++**, including the MSVC compiler and Windows SDK.
- [CMake](https://cmake.org/download/).
- [Git](https://git-scm.com/downloads).
- [vcpkg](https://github.com/microsoft/vcpkg) for GLFW and GLM.
- [GNU Make](https://www.gnu.org/software/make/) through a compatible Windows environment or distribution.
- [watchexec](https://watchexec.github.io/downloads/watchexec/) for automatic rebuilding and relaunching.
- [Visual Studio Code](https://code.visualstudio.com/) with the C/C++ and CMake Tools extensions.

The current Windows configuration uses Visual Studio Build Tools 2026, the `Visual Studio 18 2026` CMake generator, and the x64 architecture.

### Ubuntu

Install the compiler, build tools, graphics libraries, and development headers:

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  make \
  git \
  gdb \
  pkg-config \
  mesa-utils \
  libgl1-mesa-dev \
  libglfw3-dev \
  libglm-dev \
  libx11-dev \
  libxrandr-dev \
  libxinerama-dev \
  libxcursor-dev \
  libxi-dev \
  python3-venv \
  python3-pip
```

Install `watchexec`:

```bash
sudo apt install -y watchexec
```

If your Ubuntu repositories do not provide the package, install the appropriate Linux package from the [official watchexec downloads](https://watchexec.github.io/downloads/watchexec/).

Verify the tools:

```bash
g++ --version
cmake --version
make --version
git --version
watchexec --version
glxinfo -B
```

In the output of `glxinfo -B`, inspect the **OpenGL version** and **OpenGL renderer**. Confirm that the graphics driver supports the OpenGL version requested by the application. Run this check from a graphical desktop session.

## Windows Setup

### 1. Clone the repository

In PowerShell:

```powershell
mkdir C:\dev -ErrorAction SilentlyContinue
cd C:\dev
git clone git@github.com:Marc-Pozo/m_engine.git
cd m_engine
```

You can clone the repository into another directory if preferred.

### 2. Install dependencies with vcpkg

Install vcpkg following its [official setup instructions](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started).

The current CMake preset expects vcpkg at:

```text
C:/dev/vcpkg
```

If your installation is elsewhere, update the `CMAKE_TOOLCHAIN_FILE` value in `CMakePresets.json` to point to your local vcpkg toolchain file.

Install the required libraries for the 64-bit Windows target:

```powershell
C:\dev\vcpkg\vcpkg.exe install glfw3:x64-windows glm:x64-windows
```

### 3. Configure and build

From the repository root:

```powershell
cmake --list-presets
cmake --preset windows-debug
cmake --build --preset windows-debug
```

The Windows preset uses the Visual Studio 2026 generator and x64 architecture.

### 4. Run

The Debug executable is generated at:

```text
build/windows-msvc/Debug/M-Engine.exe
```

Run it from PowerShell:

```powershell
.\build\windows-msvc\Debug\M-Engine.exe
```

## Ubuntu Setup

### 1. Clone the repository

```bash
mkdir -p ~/projects
cd ~/projects
git clone git@github.com:Marc-Pozo/m_engine.git
cd m_engine
```

HTTPS can be used instead of SSH if preferred.

### 2. Generate GLAD 2 if needed

M-Engine uses GLAD to load OpenGL functions. Generated GLAD source and headers should be committed to the repository so developers can build without regenerating them.

If `third_party/glad/` is already present and contains the required generated files, skip this step.

Otherwise:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install glad2
glad --api gl:core=4.5 --out-path third_party/glad c --loader
```

For GLAD 2, the application typically includes:

```cpp
#include <glad/gl.h>
```

The generated C source is typically `third_party/glad/src/gl.c`. Confirm these paths match `CMakeLists.txt`.

Once GLAD has been generated, the Python virtual environment does not need to be activated for normal C++ builds.

### 3. Configure and build

From the repository root:

```bash
cmake --list-presets
cmake --preset ubuntu-debug
cmake --build --preset ubuntu-debug
```

The Ubuntu preset uses GCC and Unix Makefiles, with its build output placed under `build/ubuntu-debug/`.

### 4. Run

```bash
./build/ubuntu-debug/M-Engine
```

Run the application from a graphical desktop session with a working OpenGL driver.

## Build, Run, and Hot Reload

The repository includes a `Makefile` that provides consistent commands across Windows and Ubuntu. It detects the operating system and selects the appropriate CMake preset and development script.

Run these commands from the repository root.

| Command | Description |
|---|---|
| `make build` | Build the engine once using the platform's Debug preset. |
| `make dev` | Watch for file changes, rebuild, and relaunch the engine. |
| `make clean` | Clean the configured build target. |

### Hot reload

Start the development watcher:

```bash
make dev
```

The workflow uses `watchexec` with a 500 ms debounce interval. When a watched project file changes, the watcher restarts the development command, which builds the engine and launches the executable if the build succeeds.

The platform-specific scripts are:

- `scripts/dev.ps1` — Windows development build and launch.
- `scripts/dev.sh` — Ubuntu development build and launch.

The Makefile selects the appropriate script automatically.

To stop the watcher, press `Ctrl+C`.

**Requirements for hot reload:**

- `watchexec` and GNU Make must be installed and available on `PATH`.
- The platform's CMake preset must configure successfully.
- The required compiler and dependencies must be installed.
- On Linux, the application must be able to access a graphical display to open its GLFW window.

### CMake presets

The current project provides the following Debug presets:

| Preset | Platform | Build directory |
|---|---|---|
| `windows-debug` | Windows / MSVC | `build/windows-msvc/` |
| `ubuntu-debug` | Ubuntu / GCC | `build/ubuntu-debug/` |

You can use CMake directly if you do not want to use Make:

```bash
cmake --preset ubuntu-debug
cmake --build --preset ubuntu-debug
```

On Windows, use:

```powershell
cmake --preset windows-debug
cmake --build --preset windows-debug
```

Use the platform-appropriate commands. Do not select a preset intended for the other operating system.

## Project Structure

The intended project layout is:

```text
m_engine/
├── CMakeLists.txt
├── CMakePresets.json
├── Makefile
├── .gitignore
├── README.md
├── scripts/
│   ├── dev.ps1
│   └── dev.sh
├── assets/
│   ├── models/
│   ├── shaders/
│   └── textures/
├── docs/
├── third_party/
│   └── glad/
├── samples/
│   └── main.cpp
├── src/
│   └── engine/
│       ├── assets/
│       ├── core/
│       ├── editor/
│       ├── platform/
│       ├── renderer/
│       └── scene/
├── tests/
└── build/                 # Generated; do not commit
```

Some directories represent planned architecture and may not exist yet.

- `src/engine/core/`: application lifecycle, logging, and shared utilities.
- `src/engine/platform/`: windowing and platform-specific code.
- `src/engine/renderer/`: OpenGL resources, shaders, meshes, cameras, and rendering.
- `src/engine/scene/`: entities, transforms, and scene management.
- `src/engine/assets/`: asset loading and resource management.
- `src/engine/editor/`: editor UI and tools.
- `samples/`: executable used to develop and test engine features.
- `assets/`: runtime assets, including shaders, models, and textures.
- `third_party/`: vendored or generated third-party code required to build.
- `scripts/`: platform-specific development scripts.
- `docs/`: design notes and technical specifications.
- `tests/`: automated tests.
- `build/`: generated CMake build artifacts; never commit this directory.

## Git Workflow

### Branches

Use `main` for stable, buildable work. Create a short-lived branch for each feature, fix, or maintenance task:

```bash
git switch main
git pull --ff-only
git switch -c feat/camera-controls
```

Suggested branch prefixes:

- `feat/` — new functionality
- `fix/` — bug fixes
- `refactor/` — code restructuring without intended behavior changes
- `docs/` — documentation
- `test/` — tests
- `chore/` — tooling, build, and maintenance

When the work is ready, run the relevant build and tests, review the diff, and merge the branch into `main`. For a solo project, merging locally is acceptable; pull requests are useful for documenting substantial changes.

### Before committing

Build using the preset for your current platform:

**Windows:**

```powershell
make build
git status
git diff
```

**Ubuntu:**

```bash
make build
git status
git diff
```

Run tests when configured, and check that generated build output, IDE metadata, and local virtual environments are not staged:

```bash
git status --short
git diff --cached
```

### Tags and milestones

Use annotated tags for meaningful milestones:

```bash
git tag -a v0.1-bootstrap -m "Initial M-Engine build system and OpenGL application"
git push origin --tags
```

Only create a milestone tag after verifying that the tagged revision builds and runs.

## Commit Conventions

M-Engine follows a lightweight version of **Conventional Commits**:

```text
<type>(<optional scope>): <imperative summary>
```

Examples:

```text
chore: bootstrap M-Engine project
feat(renderer): render indexed 3D cube
feat(platform): add GLFW window wrapper
fix(renderer): correct depth buffer initialization
refactor(scene): separate transforms from entities
docs: document cross-platform build instructions
test(math): cover transform composition
build(cmake): add Debug presets
chore(dev): add hot-reload workflow
```

### Allowed types

| Type | Use |
|---|---|
| `feat` | Add a feature or capability |
| `fix` | Correct a bug |
| `refactor` | Restructure code without changing intended behavior |
| `perf` | Improve performance |
| `test` | Add or update tests |
| `docs` | Documentation-only changes |
| `build` | Build system or dependency changes |
| `ci` | Continuous integration changes |
| `chore` | Maintenance and project housekeeping |
| `style` | Formatting-only changes with no behavior change |

### Commit message rules

1. Use the format `type(scope): summary` when a scope helps identify the subsystem.
2. Write the summary in the imperative mood: `add camera controls`, not `added camera controls`.
3. Keep the summary concise; aim for 72 characters or fewer.
4. Use lowercase type and scope names.
5. Do not end the summary with a period.
6. Keep each commit focused on one logical change.
7. Do not commit broken intermediate work to `main`.
8. Add a body when useful to explain **why** a change was made or note trade-offs. Leave a blank line before any footer.
9. Reference issue IDs in the footer when applicable, for example `Refs: #12`.

Breaking changes can use `!` after the type or scope, with details in the commit body:

```text
feat(renderer)!: replace the material API
```

### Suggested commit sequence for the initial setup

```bash
git add .gitignore CMakeLists.txt CMakePresets.json Makefile README.md
git add scripts samples assets docs tests third_party
git commit -m "chore: bootstrap M-Engine project"
```

Adjust the staged paths to match the files that actually exist. Do not stage files that are absent or generated build output.

## Roadmap

1. **Bootstrap:** CMake build, GLFW window, GLAD initialization, OpenGL context.
2. **First renderer:** shaders, vertex/index buffers, depth testing, and a rotating cube.
3. **Renderer structure:** shader, mesh, camera, and buffer abstractions.
4. **Engine core:** application lifecycle, logging, and input handling.
5. **Scene system:** entities, transforms, and scene updates.
6. **Asset pipeline:** load shader files, meshes, and textures from `assets/`.
7. **Editor foundation:** Dear ImGui, scene viewport, and basic inspection tools.
8. **Editor workflow:** entity selection, transform editing, and scene persistence.
9. **Quality:** automated tests, profiling, error handling, and documentation.

Treat this as an evolving roadmap rather than a promise that every feature is already implemented.