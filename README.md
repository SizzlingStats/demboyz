demboyz
=======

we dem boyz


### Using Vagrant

    # Launch VM
    vagrant up
    vagrant ssh
    cd /vagrant

    # Install Linux dependencies (first time setup)
    ./vagrant.sh

    # Build demboyz
    cd premake
    ./gmake.sh && cd gmake
    make

    # Disconnect session and stop VM
    exit
    vagrant halt
