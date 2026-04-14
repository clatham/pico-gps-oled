# Lesson 1 - Setup an environment to build using the Raspberry Pi Pico C/C++ SDK

## Step 1 - Enable WSL2 and Install Ubuntu LTS on your Windows 11 machine

Click the Windows "Start" button, and without selecting anything else, type "Powershell".  On the right side, under the Windows Powershell name and logo, select "Run as Administrator".  When asked if you'd like to allow it to make changes to your system, select "Yes".

In Powershell, run the following command:
```
wsl --install Ubuntu-24.04
```
This will enable WSL2, and it will install the long-term support version of Ubuntu (currently 24.04).  To view all the possible Linux distros, you can run the command:
```
wsl --list --online
```

In Windows Terminal, you can now click the drop-down arrow on the menu to open a new tab.  Select "Ubuntu" to run your newly installed Ubuntu shell.


## Step 2 - Update and Patch your Ubuntu Installation

It is a good idea to periodically update and patch the software and libraries in your Ubuntu installation.  You can do this with the following commands:
```
sudo apt update
```
This will update the list of packages available to the apt utility.  If it doesn't know about new packages, then you can't use them.

```
sudo apt upgrade
```
This will upgrade the packages that you already have installed (and their dependencies) to the latest version available.

```
sudo apt autoremove
```
This will remove packages that are no longer dependencies of any package.


## Step 3 - Install Required Packages

We will need several packages installed to use the C/C++ SDK and cross-compile for Pi Pico.  To install them, use the following commands:
```
sudo apt update
sudo apt install -y cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential git
```


## Step 4 - Download the Pi Pico C/C++ SDK

We need to clone the Pico SDK repo somewhere.  Since I like to keep all of my git projects under ~/git, I will run the following commands:
```
mkdir ~/git
cd ~/git
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
```

This pulls the entire Pico SDK repo into ~/git/pico-sdk, and it updates/pulls submodules used by it as well.


## Step 5 - Setup your Environment

Finally, tell scripts where to find the Pico SDK by setting the environment variable PICO_SDK_PATH to point to the installed location.  Edit your ~/.bashrc file, and the following to the bottom of the file:
```
export PICO_SDK_PATH=~/git/pico-sdk
```

After you save the file, either close and reopen the shell, or run the following command:
```
source ~/.bashrc
```


## Conclusion

You're now ready to build binaries for your Raspberry Pi Pico using the C/C++ SDK.


## Errata

File paths in Linux start at the root directory or "/".  If a path begins with "/" then it is an absolute path from the root.  If you want to start from the current directory, then use "./" instead.

"~" is an alias for the current user's home directory.  So if you "cd ~" then check the current directory with "pwd", it will tell you something like "/home/chris".

"sudo" means do this as the superuser/root.  Be careful what you sudo, as you may not be able to undo it.

You can edit files right in your shell with a text-based editor like Vi.  Vi is not a what-you-see-is-what-you-get (wysiwyg) editor.  It takes a little effort to learn the commands and to get used to its modes.
