
#!/usr/bin/env bash

set -u

echo
echo "=== Building M-Engine (Ubuntu Debug) ==="

if ! cmake --build --preset ubuntu-debug; then
    echo "Build failed. Waiting for the next file change..."
    exit 1
fi

echo
echo "=== Launching M-Engine ==="

exec ./build/ubuntu-debug/M-Engine
