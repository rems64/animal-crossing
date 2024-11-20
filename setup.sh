#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "Setting up development environment..."

# Check OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if ! command -v brew &> /dev/null; then
        echo "Installing Homebrew..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    
    brew install glfw glm xmake
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if command -v apt-get &> /dev/null; then
        # Debian/Ubuntu
        sudo apt-get update
        sudo apt-get install -y build-essential libglfw3-dev libglm-dev
    elif command -v dnf &> /dev/null; then
        # Fedora
        sudo dnf install -y gcc-c++ glfw-devel glm-devel
    elif command -v pacman &> /dev/null; then
        # Arch Linux
        sudo pacman -Syu --noconfirm
        sudo pacman -S --noconfirm base-devel glfw-x11 glm
    fi

    # Install xmake
    if ! command -v xmake &> /dev/null; then
        echo "Installing xmake..."
        bash <(curl -fsSL https://xmake.io/shget.text)
    fi
fi

# Create build directory
mkdir -p build

# Run xmake
echo "Running xmake..."
xmake f -m release
xmake

echo -e "${GREEN}Setup complete!${NC}" 