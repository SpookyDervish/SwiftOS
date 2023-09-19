# SwiftOS
A from scratch hobbyist operating system.

This operating system is **DEFINITELY** not ready for production environments. Watch out for bugs!

Hello! I have remade my original operating system from scratch in C++. It runs on the i386 architecture. I hope this to become a long lasting project I work on for a while as I don't want this to be another failure. This operating system is intended to be an educational resource for future beginner operating system developers.

## Current Features
- GRUB bootloader
- Kernel
- Networking with the amd_am79c973 (Basic HTTP, TCP, UDP, ICMP, ethernet, IP4, ARP)
- FAT32 file system
- VGA 320x200 with 256 colour display
- ATA drive support
- MSDOS partition table
- Dynamic memory manager (malloc, free)
  
## Planned Features
  - [ ] Filesystem API
    - [x] Fat32 filesystem
    - [x] Partition table
    - [ ] FS API for programs
  - [ ] Moving from VGA graphics to SVGA graphics or better
  - [ ] Popuating all the syscalls
    - [ ] IO syscalls
    - [ ] FS syscalls
    - [ ] Graphics syscalls
    - [ ] Audio syscalls  
    And so on...
  - [ ] Sound driver
  - [ ] Full desktop
    - [ ] Full UI API
      - [x] Core classes for the gui framework
  - [ ] Operating system website
  - [ ] USB support
    - [ ] Keyboard USB
    - [ ] Mouse USB
    - [ ] Audio USB
    - [ ] Plugging in USBs and reading
  - [ ] A logo for the operating system
  - [ ] Assets
    - [ ] Logo
    - [ ] File type icons
    - [ ] Program icons
    - [ ] Desktop backgrounds
    - [ ] In app images
  - [ ] Screenshots
  
## Project Layout
- **drivers** | Drivers for interfacing with hardware
- **filesystem** | FAT32 implementation, these will possibly be updated with implementations of other filesystems in the future
- **gui** | Contains the files for the gui framework
- **hardwarecommunication** | Interrupts, ports, pci implementation
- **net** | Networking inmplementation for the amd_am79c973, will possibly be extended with further implementations for different networking hardware
- **kernel.cpp** | SwiftOS's kernel. Manages memory, hardware, drivers, and instantiates all the components of SwiftOS  
  
**Everything else is just implementations.**
    
## Building
To build SwiftOS, you will need the dependencies.

SwiftOS was made on Linux, I'm not sure about Windows or MacOS, you will need to install the dependencies.

### Ubuntu:
```
sudo apt-get install g++ binutils libc6-dev-i386 qemu grub-legacy xorriso
```

### Solus:
```
sudo eopkg install g++ binutils qemu grub2 libisoburn virtualbox
```

### Other:
Use your system package manager to get the dependencies, the package names should be relatively the same.

## Running
Next, if you want to use QEMU, type:
```
make run-qemu
```
If you'd prefer to use VirtualBox, create a new virtual machine and name it "SwiftOS VirtualBox VM", type:
```
make run
```
and then set the virtual machines iso as the one created in the project directory and rerun the command.

Each of these commands will also create an iso file in the project directory, you can use this in any other virtual machine software, or burn it to a USB.

## Real Hardware
**QUICK DISCLAIMER**  
I have **not** tested this on real hardware! Real hardware might not like it.

But with that out of the way...  
To those who still want to test it make sure you use the correct hardware supported by SwiftOS.

## Credits
I would like to give a big thank you to the following people:
- @writeyourownoperatingsystem on YouTube | I learnt a lot from your tutorial series and I'm now using your tutorials as a base that i can build upon

## Contributing
Any help is extremely appreciated, feel free to make pull requests and other contributions, then I just have to moderate them.

# Thanks for reading!
