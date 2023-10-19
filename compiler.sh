#!/bin/bash

# List of packages to install
packages=(
    build-essential
    gdb  # GNU Debugger
    cmake  # Build system generator
    git  # Version control system
    libboost-all-dev  # Boost C++ Libraries
    libssl-dev  # OpenSSL development libraries
    libncurses5-dev  # ncurses library
    libyaml-cpp-dev  # YAML parser and emitter for C++
        # Add more packages as needed
)
# Update package list and upgrade existing packages
sudo apt update -y
sudo apt upgrade -y
# Install the specified packages
for package in "${packages[@]}"; do
    sudo apt install -y "$package"
done

sudo snap install -y --classic code
# Optionally, perform additional configuration or setup here
# ...
g++ -o loadernode /root/loadernode/srccpp/loadernode.cpp
