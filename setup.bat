@echo off
setlocal enabledelayedexpansion

echo Setting up development environment...

:: Check if we have admin rights
net session >nul 2>&1
if %errorLevel% == 0 (
    set ADMIN=1
) else (
    set ADMIN=0
)

:: Check for Visual Studio installation
where cl >nul 2>&1
if %errorLevel% neq 0 (
    echo Visual Studio not found. Please install Visual Studio 2022 with C++ desktop development workload
    echo Download from: https://visualstudio.microsoft.com/downloads/
    pause
    exit /b 1
)

:: Install xmake if not present
where xmake >nul 2>&1 
if %errorLevel% neq 0 (
    echo Installing xmake...
    powershell -Command "Invoke-Expression (Invoke-WebRequest 'https://xmake.io/psget.text' -UseBasicParsing).Content"
)

:: Create build directory
if not exist "build" mkdir build

:: Run xmake
echo Running xmake...
xmake f -m release
xmake

echo Setup complete!
pause