#!/bin/sh

# Update package list
sudo apt-get update

# Add the repository for the latest GCC
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y

# Update package list again after adding the repository
sudo apt-get update

sudo update-alternatives --remove-all gcc 
sudo update-alternatives --remove-all g++

# Install g++-9 and related build tools
sudo apt-get install -y build-essential g++-9

# Set g++-9 as the default g++
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 60
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 60

# Verify installation
g++ --version
gcc --version

# Install g++-9 multilib support if needed
sudo apt-get install -y g++-9-multilib
