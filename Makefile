
.PHONY: dev build clean

ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    PRESET := windows-debug
    DEV_CMD := powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/dev.ps1
else
    PLATFORM := linux
    PRESET := ubuntu-debug
    DEV_CMD := bash scripts/dev.sh
endif

dev:
	watchexec --restart --debounce 500ms -- $(DEV_CMD)

build:
	cmake --build --preset $(PRESET)

clean:
	cmake --build --preset $(PRESET) --target clean

.PHONY: dev build clean

ifeq ($(OS),Windows_NT)
    PLATFORM := windows
    PRESET := windows-debug
    DEV_CMD := powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/dev.ps1
else
    PLATFORM := linux
    PRESET := ubuntu-debug
    DEV_CMD := bash scripts/dev.sh
endif

dev:
	watchexec --restart --debounce 500ms -- $(DEV_CMD)

build:
	cmake --build --preset $(PRESET)

clean:
	cmake --build --preset $(PRESET) --target clean
