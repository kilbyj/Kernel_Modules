Kernel_Modules
==============

I am using this repository to write the kernel module on my windows machine and then copy it over to my RPi to test building.


All we had to do to compile the kernel module for the zedboard was to download the header files from aptitude.

After we had the headers we just used the Makefile that we wrote that linked against the header files that we downloaded and it would compile the module for us.

Installing Kernel Module on Zedboard
====================================
Step 1
  Get the header files using apt-get or aptitude.
      <code>sudo aptitude install linux-headers-YOUR_KERNEL_VERSION-generic</code>
  We just used tab completion to get the files for use so we typed just linux-headers- and hit tab to see our options
  
Step 2
  Create the Makefile as it is shown in the Devel branch.
  
Step 3
  Create the module that you want to compile. We just used perf_counter_test_a9.c
    Make sure to put in some printks so that you can know if your module has been registered or not.
  
Step 4
  Make the module by typing "make"
  
Step 5
  Insert the module using
    <code>sudo insmod ./perf_counter_test_a9.ko</code>

Step 6
  Verify the module has been inserted.
    <code>sudo dmesg | tail -10</code>

Step 7
  If everything is done correctly you can now use the kernel module and access the performance counters on the zedboard form userspace.
