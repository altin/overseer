# overseer
System Usage Management for Linux.

## About
###### Authors
* [Altin Rexhepaj](https://github.com/altin)
* [Randy Taylor](https://github.com/aclonegeek)

###### Purpose
overseer is a course project designed for COMP 3000 (Operating Systems) at Carleton University during the Winter of 2018. It aims to make use of learned course material which includes, but is not limited to:
* Files
* /proc File system
* Inter Process Communication (IPC)
  * Signals
* Memory Allocation/Deallocation
* Processes and system calls

###### Instructions
!!! ALL STEPS ASSUME YOU ARE IN THE OVERSEER DIRECTORY !!!  
!!! AND AN ADMINISTRATIVE USER !!!

Ensure python3 is installed.  
Install tkinter: sudo apt-get install python3-tk  
Run the pcontrol GUI: python3 pcontrol_gui.py  
- Note: After saving,the config (config.json) is stored in the overseer/src/pcontrol directory.  

Run the project:  
sudo ./run.sh install  
Add a user (sudo useradd -g overseen -m NAME_HERE)  
Provide a password (sudo passwd NAME_HERE)  
Reboot your system  
Done!  

Uninstall the project:  
sudo ./run.sh uninstall  

###### Third-Party Libraries
* [frozen](https://github.com/cesanta/frozen)