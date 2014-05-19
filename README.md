Kernel_Modules
==============

I am using this repository to write the kernel module on my windows machine and then copy it over to my RPi to test building.


All we had to do to compile the kernel module for the zedboard was to download the header files from aptitude.

After we had the headers we just used the Makefile that we wrote that linked against the header files that we downloaded and it would compile the module for us.
