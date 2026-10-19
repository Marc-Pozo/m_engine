
$ErrorActionPreference = "Stop"

Write-Host "`n=== Building M-Engine (Windows Debug) ===" -ForegroundColor Cyan

cmake --build --preset windows-debug

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed. Waiting for the next file change..." -ForegroundColor Red
    exit 1
}

$executable = Join-Path $PSScriptRoot "../build/windows-msvc/Debug/M-Engine.exe"
$executable = [System.IO.Path]::GetFullPath($executable)

if (-not (Test-Path $executable)) {
    Write-Error "Executable not found: $executable"
    exit 1
}

Write-Host "`n=== Launching M-Engine ===" -ForegroundColor Green
& $executable
