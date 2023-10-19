#!/bin/bash

# Update the system
sudo dnf update -y

# Install essential development tools and libraries
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y cmake
sudo dnf install -y git
sudo dnf install -y boost-devel
sudo dnf install -y openssl-devel
sudo dnf install -y ncurses-devel
sudo dnf install -y yaml-cpp-devel

echo "C++ development packages and tools installation complete."

g++ -o workerloadernode ./loadernode/srccpp/loadernode.cpp
g++ -o serverloadernode ./loadernode/srccpp/server.cpp
echo "C++ development packages installation and compilation complete."