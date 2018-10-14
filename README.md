# overseer
Overseer is a system usage management tool designed for Ubuntu. It creates a custom user group with dynamic web and process restrictions for the purpose of controlling computer use.

## About
###### Authors
* [Altin Rexhepaj](https://github.com/altin)
* [Randy Taylor](https://github.com/aclonegeek)

###### Purpose
Overseer is a course project designed for COMP 3000 (Operating Systems) at Carleton University during the Winter of 2018. It aims to make use of learned course material which includes, but is not limited to:
* Files
* /proc File system
* Inter Process Communication (IPC)
  * Signals
* Memory Allocation/Deallocation
* Processes and system calls

###### Instructions
**ALL STEPS ASSUME YOU ARE IN THE OVERSEER DIRECTORY AND A SUDO USER** 

###### Dependencies
1. Ensure Python3 is installed.  
2. Install tkinter: `sudo apt-get install python3-tk`  
3. Run the pcontrol GUI: `python3 pcontrol_gui.py`  
**Note: After saving,the config (config.json) is stored in the overseer/src/pcontrol directory.** 

###### Install
1. `sudo ./run.sh install`
2. Add a user `sudo useradd -g overseen -m NAME_HERE`  
3. Provide a password `sudo passwd NAME_HERE` 
4. Reboot your system  
Done! The `overseen` user group should now be running an OpenDNS web filter configuration, as well as the process monitor with the predefined process blacklist created from the sudo user.  

###### Uninstall  
`sudo ./run.sh uninstall`  

###### Third-Party Libraries
* [frozen](https://github.com/cesanta/frozen)
