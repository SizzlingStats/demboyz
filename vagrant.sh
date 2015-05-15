#!/bin/bash

# http://askubuntu.com/a/497033
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update

sudo apt-get install build-essential -y

sudo apt-get install gcc-4.9 g++-4.9 -y
sudo apt-get install gcc-4.9-multilib g++-4.9-multilib -y

sudo apt-get install clang-3.6++ -y
sudo ln -s /usr/bin/clang++-3.6 /usr/bin/clang++
