Basic model of a simple 32-bit operating system, with an interrupt handler.
Before starting to study the topic or read the repository, I recommend that you familiarize yourself with the Intel documentation, specifically the x86 architecture family:
https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html

It is also recommended that you familiarize yourself with the basic theory of creating operating systems: https://wiki.osdev.org/Expanded_Main_Page

NASM Assembly Language Reference: https://ece-research.unm.edu/jimp/310/nasm/nasmdoc0.html

In this repository I tried to implement the most basic and necessary functionality for the OS to work.
To run the image, you will need any x86 emulator. I recommend QEMU: https://www.qemu.org/

For Linux: ```sudo apt install qemu```

In order to compile the kernel, you will need a cross compiler: 
https://wiki.osdev.org/GCC_Cross-Compiler#Prebuilt_Toolchains
You will also need an assembly language compiler: https://www.nasm.us/ 
Or you can download it all in one place via MSYS2: https://www.msys2.org/
In the MSYS2 console: ```$ pacman -S mingw-w64-ucrt-x86_64-gcc ``` 
```pacman -S --needed mingw-w64-i686-toolchain```


Links to additional information that I used as a theory:

-http://www.brokenthorn.com/Resources/OSDevIndex.html

-https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html

-https://www.reddit.com/r/osdev/

-https://wiki.osdev.org/Boot_Sequence

-https://github.com/tuhdo/os01

-https://littleosbook.github.io/

-https://wiki.osdev.org/Bare_Bones

-https://wiki.osdev.org/Expanded_Main_Page
