# cisco-cli-interface

## Introduction

Hello! Will you help me build a C application please? It shouldn't be too difficult, and I am not looking for anything that is overly complex.

In short, I am looking to write an application that interfaces with a Cisco Switch though a serial port. The switch will be running Cisco OS and the computer will be using screen to connect to the switch.

## Use Case

* As a developer, I want a way to view all the files and folders in the flash:/ directory on a cisco switch as a single tree view.
* As a developer, I want to be able to select files and folders individually to be deleted for me from a single tree view.
* As a developer, I want to press a button and delete all files and folders I have selected to be deleted from a single tree view.

## Tech Stack

* I want this program written in C and compiled to a binary
* If a UI is used, I would like it to be something simple like curses, or ncurses.
* If no UI is used, I woudl like it to be an easy to navigate text menu that has some interactive ability for:
  * Selecting what files to delete
  * Scrolling through file tree view
* I would like to use screen for connecting to the Cisco switch via serial, unless you feel there is a good C library for doing this

## Cisco Switch Output

This section talks about the Cisco switch interface that this program will be interacting with.

### Starting up communication with switch

On startup, the operator of the switch manually has to override the boot sequence, which can take up to a couple minutes. Upon that finishing, something like this is sent over the serial line:

``` text
The password-recovery mechanism is enabled.

The system has been interrupted prior to initializing the
flash filesystem.  The following commands will initialize
the flash filesystem, and finish loading the operating 
system software:

    flash_init
    boot
```

The switch is now in a pre-boot environment where the bios is initialized, but the base image has not been booted.

The important line is that flash_init is sent over the serial line. Upon receiving this, the application should send the following command over the serial line to indiciate to the switch they would like to initialize the flash (This will allow the application the ability to view the contents of the flash:/ directory.)

```text
flash_init
```

The switch will print lots of different messages over the serial line, but we are waiting for characters to stop printing and a command line with a <name>: and a blinking cursor should appear. Something like:

```text
switch: 
```

The switch is now ready to be interacted with!

### Interacting with the switch

This section is to give you some context on some commands that are available to use to talk with the Cisco switch and provide you some examples.

#### dir

dir as an empty command will show you what filesystems are available.

``` text
switch: dir

List of filesystems currently registered:

                  bs[0]: (read-only)
               flash[3]: (read-write)
              xmodem[4]: (read-only)
                null[5]: (read-write)

```

#### dir flash:/

This is how you see the flash:/ filesystem

```text
switch: dir flash:/
Directory of flash://

    2  -rwx  1429      <date>               syslog.log
    3  -rwx  143410    <date>               startup-config.back
    4  -rwx  18282880  <date>               c3750-ipservicesk9-mz.150-2.SE11.bin
    5  -rwx  4223      <date>               tsart
    6  -rwx  616       <date>               vlan.dat
    7  -rwx  4120      <date>               multiple-fs
    8  -rwx  87925     <date>               config.text
    9  -rwx  87987     <date>               config.text.backup
   10  -rwx  8986      <date>               private-config.text.backup
   11  -rwx  8986      <date>               private-config.text

9209344 bytes available (18788864 bytes used)
```

#### dir flash:/<folder name>

This is how you see into a sub directory. This also stacks (e.g. dir flash:/<folder name>/<folder name>):

``` text
switch: dir flash:/temp
Directory of flash:/temp/

   14  -rwx  616       <date>               vlan.dat.backup

9205760 bytes available (18792448 bytes used)
```


#### delete flash:/temp/vlan.dat.backup

This is used to delete a file (example shows from a subfolder, but can also be used is flash:/ base directory)

```text
switch: delete flash:/temp/vlan.dat.backup
Are you sure you want to delete "flash:/temp/vlan.dat.backup" (y/n)?y
File "flash:/temp/vlan.dat.backup" deleted
```

Its imporant to note that "y" will need to be entered by the application to confirm the deletion of the selected file.

#### rmdir flash:/temp

This is used to delete a directory (this can be used in subfolders as well as the base flash:/ directory):

```text
switch: rmdir flash:/temp
Are you sure you want to remove "flash:/temp" (y/n)?y
Directory "flash:/temp" removed
```

Its imporant to note that "y" will need to be entered by the application to confirm the deletion of the selected directory.


#### Additional CLI information

* All directories must be entirely empty before being deleted
* Any files ending in ".bin" extention are a binary image. These really shouldn't be deleted by the user of the application, so if its selected to be deleted, the user should be prompted with a confirmation to verify they do indeed want to delete the binary
* Any files with ".tar" or ".pkg" should be treated with the same as a ".bin" file extentions

## User Interface for Application

The user will need to interact with the application. I would like the user to see:
* When the application is booting up (waiting for the flash to be initialized on the switch)
* Once booted:
  * Option to see tree view - Just shows tree
  * Option to select files to delete - shown as a tree view
    * If a folder is selected, that should select the folder and all files in it to delete
    * Visual indication that a folder or file is selected
    * When delete is exectuted in the application, feedback is presented to the user of success or failure for each file or folder. A failed delete will print something like "File "flash:/temp" not deleted for file and "Directory "flash:/temp" not removed -- no such file or directory"
    * if a /bin, .tar, or .pkg file is selected, the user must confirm each individual file should be deleted. If not, the deletion command is cancelled and never sent to the switch for the whole batch

There should be an overarching "menu navigation" ability so you can go forward and back through pages. This is to create a layered approach to the application so future functionality and pages may be added at a later date.


## Additional Info

* The system running this binary will be linux, meaning the serial connection ports will be something like /dev/ttyUSB0 or /dev/ttyUSB0
