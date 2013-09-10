#!/bin/bash

# References
# * https://wiki.ubuntu.com/DebootstrapChroot
# * http://wiki.maemo.org/Easy_Debian#Startup_scripts
# * https://wiki.debian.org/chroot
# * https://gist.github.com/jeremyckahn/3195325
# * http://superpiadventures.com/2012/07/development-environment/

TARGETDIR=robocakes-rasp
TARGETPATH=/var/${TARGETDIR}

# Check for sudo
if [ "$UID" -ne "$ROOT_UID" ]; then
    echo "You need to run this setup with sudo."
    exit 1
fi

# Update apt - just in case
sudo apt-get update

# Install qemu-user-static
if ! dpkg -s qemu-user-static &> /dev/null; then
    echo "Installing qemu-user-static"
    sudo apt-get -y install qemu-user-static
fi

# Install qemu-debootstrap
if ! dpkg -s qemu-debootstrap &> /dev/null; then
    echo "Installing qemu-debootstrap"
    sudo apt-get -y install qemu-debootstrap
fi

# Create directory for chroot /var/robocakes-rasp
if [ -e "${TARGETPATH}" ]; then
    echo "${TARGETPATH} already exists. Cannot continue."
    exit 1
else
    sudo mkdir ${TARGETPATH}
fi

#Change to working directory
cd /var

# Add gpg key for raspbian 
#wget http://archive.raspbian.org/raspbian.public.key -O - | sudo apt-key add -

# Create the chroot
sudo qemu-debootstrap --keyring /etc/apt/trusted.gpg --arch armhf wheezy $TARGETDIR http://archive.raspbian.org/raspbian

# Mount partitions
sudo mount -t proc proc ${TARGETPATH}/proc
sudo mount -t sysfs sysfs ${TARGETPATH}/sys
sudo mount -o bind /dev ${TARGETPATH}/dev

sudo LC_ALL=C chroot ${TARGETPATH}

# Add a name to /etc/debian_chroot (shows up in the prompt)
echo raspbian > etc/debian_chroot

# Source the root bashrc to update prompt
source /etc/bash.bashrc

# Add the Raspbian repo and set up a GPG key.
echo "deb http://archive.raspbian.org/raspbian wheezy main" >> /etc/apt/sources.list
wget http://archive.raspbian.org/raspbian.public.key -O - | apt-key add -
apt-get update

# Install a few essentials
apt-get -y install sudo vim

# Create a pi user
useradd -m -s /bin/bash pi

# Switch to the new user
su - pi

#TODO list
echo "### TODO List ###"
echo " * Configure sudo for pi user"
echo " * Install build packages"
echo " * Git"
echo " * SSH"
echo ""

echo "Welcome to your new Raspbian chroot. Type 'exit' to exit."
