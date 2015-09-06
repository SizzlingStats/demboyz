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

### With Visual Studio 2013

    # Generate vs 2013 project
    cd premake
    vs2013.bat
    
Open generated VS solution at premake/vs2013/demboyz.sln
