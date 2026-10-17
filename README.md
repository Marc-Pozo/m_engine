# M-Engine

A learning-focused 3D game engine and editor built in C++ with OpenGL. The project is developed on Ubuntu and uses CMake for reproducible builds.

> **Current status:** Bootstrap stage. The initial goal is a working OpenGL application, followed by a first 3D renderer and progressively more engine/editor systems.

## Table of Contents

- [Goals](#goals)
- [Technology Stack](#technology-stack)
- [Prerequisites](#prerequisites)
- [Ubuntu Setup](#ubuntu-setup)
- [Build and Run](#build-and-run)
- [Project Structure](#project-structure)
- [Git Workflow](#git-workflow)
- [Commit Conventions](#commit-conventions)
- [Development Guidelines](#development-guidelines)
- [Roadmap](#roadmap)
- [Troubleshooting](#troubleshooting)

## Goals

M-Engine is a solo-developer learning and portfolio project. Its goals are to:

- Build a 3D rendering pipeline using modern OpenGL.
- Develop a clear, modular C++ engine architecture.
- Create an editor for inspecting and working with scenes.
- Use reliable build, testing, and version-control practices.

## Technology Stack

| Area | Technology |
|---|---|
| Language | C++20 |
| Graphics API | OpenGL 4.5 Core Profile |
| Window and input | GLFW |
| OpenGL function loading | GLAD 2 |
| Mathematics | GLM |
| Shaders | GLSL 4.50 |
| Build system | CMake |
| Build generator | Ninja |
| Compiler | GCC or Clang |
| Version control | Git |

The machine's graphics driver must support OpenGL 4.5. The application prints the OpenGL version reported by the driver when it starts.

## Prerequisites

On Ubuntu, install the compiler, build tools, graphics libraries, and development headers:

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  ninja-build \
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

Verify the key tools are available:

```bash
g++ --version
cmake --version
ninja --version
git --version
glxinfo -B
```

In the output of `glxinfo -B`, check the **OpenGL version** and **OpenGL renderer**. Ensure the reported version is compatible with OpenGL 4.5.

## Ubuntu Setup

### 1. Clone the repository

```bash
mkdir -p ~/projects
cd ~/projects
git clone git@github.com:Marc-Pozo/m_engine.git
cd m_engine
```

If you use HTTPS instead of SSH, clone with the HTTPS URL shown by GitHub.

### 2. Generate GLAD 2

M-Engine uses GLAD to load OpenGL functions. The generated GLAD source and headers should be committed to the repository so other developers can build without regenerating them.

If `third_party/glad/` is not already present in the repository, generate it:

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install glad2
glad --api gl:core=4.5 --out-path third_party/glad c --loader
```

For GLAD 2, the header used by the application is typically:

```cpp
#include <glad/gl.h>
```

The generated C source is typically `third_party/glad/src/gl.c`. Confirm these paths match the CMake configuration before building. You do **not** need to activate the Python virtual environment for normal C++ builds once GLAD has been generated.

### 3. Configure the project

The repository provides CMake presets for Debug and Release builds:

```bash
cmake --preset debug
```

If presets are not available yet, use the equivalent direct configuration command:

```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
```

### 4. Build

With presets:

```bash
cmake --build --preset debug
```

Or, if you configured with the direct command:

```bash
cmake --build build
```

### 5. Run

The executable location depends on the chosen build configuration:

```bash
./build/debug/M-Engine
```

For the direct configuration command, run:

```bash
./build/M-Engine
```

### 6. Release build

```bash
cmake --preset release
cmake --build --preset release
```

Run the Release executable at `./build/release/M-Engine` if that is the binary path configured by the project.

## Project Structure

The intended layout is:

```text
m_engine/
├── CMakeLists.txt
├── CMakePresets.json
├── .gitignore
├── README.md
├── assets/
│   ├── models/
│   ├── shaders/
│   └── textures/
├── docs/
├── third_party/
│   └── glad/
├── src/
│   ├── engine/
│   │   ├── assets/
│   │   ├── core/
│   │   ├── editor/
│   │   ├── platform/
│   │   ├── renderer/
│   │   └── scene/
│   └── samples/
├── tests/
└── build/                 # Generated; do not commit
```

- `src/engine/core/`: application lifecycle, logging, and shared utilities.
- `src/engine/platform/`: windowing and platform-specific code.
- `src/engine/renderer/`: OpenGL resources, shaders, meshes, cameras, and rendering.
- `src/engine/scene/`: entities, transforms, and scene management.
- `src/engine/assets/`: asset loading and resource management.
- `src/engine/editor/`: editor UI and tools.
- `src/samples/`: executable used to develop and test engine features.
- `assets/`: runtime assets, including shader files.
- `third_party/`: vendored/generated third-party code required to build.
- `docs/`: design notes and technical specifications.
- `tests/`: automated tests.

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

```bash
git status
git diff
cmake --build --preset debug
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
docs: document Ubuntu build instructions
test(math): cover transform composition
build(cmake): add Debug and Release presets
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
8. Add a body when useful to explain **why** a change was made or note trade-offs. Wrap up the body with a blank line before any footer.
9. Reference issue IDs in the footer when applicable, for example `Refs: #12`.

Breaking changes can use `!` after the type or scope, with details in the commit body:

```text
feat(renderer)!: replace the material API
```

### Suggested commit sequence for the initial setup

```bash
git add .gitignore CMakeLists.txt CMakePresets.json README.md
git add src assets docs tests third_party
git commit -m "chore: bootstrap M-Engine project"
```

Adjust the staged paths to match the files that actually exist. Do not stage files that are absent or generated build output.

## Development Guidelines

- Prefer small, cohesive classes with explicit ownership and lifecycle.
- Keep OpenGL-specific implementation inside the renderer rather than spreading it across scene or editor code.
- Check shader compilation and program linking; include useful diagnostic logs.
- Use RAII for C++ resources where practical.
- Keep build and runtime configuration reproducible.
- Avoid committing `build/`, `.venv/`, compiled binaries, editor-specific files, or machine-local configuration.
- Vendor or commit generated loader code only where needed to make builds reproducible; do not commit unrelated package caches.
- Update documentation when commands, dependencies, directory layout, or architecture change.

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

## Troubleshooting

### CMake cannot find GLFW or GLM

Confirm the Ubuntu development packages are installed:

```bash
sudo apt install libglfw3-dev libglm-dev
```

Then rerun configuration:

```bash
cmake --preset debug
```

### GLAD header or source cannot be found

Check that the generated files exist:

```bash
ls third_party/glad/include/glad/gl.h
ls third_party/glad/src/gl.c
```

If they are missing, follow [Generate GLAD 2](#2-generate-glad-2). Make sure CMake includes `third_party/glad/include` and compiles `third_party/glad/src/gl.c`.

### OpenGL version is too low

Run:

```bash
glxinfo -B
```

Update or configure the appropriate graphics driver for your GPU. If running inside a virtual machine or remote desktop, verify that 3D acceleration and the required OpenGL version are available.

### The window opens and immediately closes or shows a blank screen

Run from a terminal to see diagnostics:

```bash
./build/debug/M-Engine
```

Check the reported OpenGL version, shader compilation logs, and application output. Verify that the active build path matches the preset or CMake command you used.

---

## License

Add a `LICENSE` file before distributing the project. Until then, no license is explicitly granted by this README.
