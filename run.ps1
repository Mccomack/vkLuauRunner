$ErrorActionPreference = "Stop"

& "$PSScriptRoot\build.ps1" "Release"

New-Item -ItemType Directory -Force -Path "runtest" | Out-Null
Push-Location runtest
try {
    & "..\build\Release\src\program.exe"
} finally {
    Pop-Location
}
