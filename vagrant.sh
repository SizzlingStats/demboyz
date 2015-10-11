#!/bin/bash

# http://askubuntu.com/a/497033
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update

# Remove X11 cruft
sudo apt-get autoremove -y

# Keep things up to date
sudo apt-get upgrade -y

packages=""; # Start with nothing; can re-order rest to heart's content

packages="$packages build-essential"
packages="$packages gcc-4.9 g++-4.9"
packages="$packages gcc-4.9-multilib g++-4.9-multilib"
packages="$packages clang-3.6++"

sudo apt-get install -y $packages

# Why doesn't clang do this for us?
[ ! -e /usr/bin/clang++ ] && sudo ln -s /usr/bin/clang++-3.6 /usr/bin/clang++

# Build it!
cd /vagrant/premake
/bin/bash gmake.sh && { cd gmake; make; }
