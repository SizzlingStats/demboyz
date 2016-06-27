#!/bin/sh

# Keep things up to date
sudo apt-get update
sudo apt-get upgrade -y

# Install g++ dependencies 
sudo apt-get install -y build-essential g++-4.8 g++-4.8-multilib

# Build it!
cd /vagrant/premake
./gmake.sh && { cd gmake; make; }
