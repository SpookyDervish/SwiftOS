# Debian
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install qemu grub-legacy xorriso
# Solus
# sudo eopkg install g++ binutils qemu grub2 libisoburn virtualbox

SRCDIR = src
OBJDIR = obj
ISODIR = iso

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -fno-stack-protector
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = $(OBJDIR)/boot.o \
          $(OBJDIR)/gdt.o \
		  $(OBJDIR)/memorymanagement.o \
          $(OBJDIR)/drivers/driver.o \
          $(OBJDIR)/hardwarecommunication/port.o \
          $(OBJDIR)/hardwarecommunication/interruptstubs.o \
          $(OBJDIR)/hardwarecommunication/interrupts.o \
          $(OBJDIR)/hardwarecommunication/pci.o \
		  $(OBJDIR)/syscalls.o \
		  $(OBJDIR)/multitasking.o \
		  $(OBJDIR)/drivers/amd_am79c973.o \
          $(OBJDIR)/drivers/keyboard.o \
          $(OBJDIR)/drivers/mouse.o \
          $(OBJDIR)/drivers/vga.o \
		  $(OBJDIR)/drivers/ata.o \
		  $(OBJDIR)/gui/widget.o \
          $(OBJDIR)/gui/window.o \
          $(OBJDIR)/gui/desktop.o \
		  $(OBJDIR)/net/etherframe.o \
		  $(OBJDIR)/net/arp.o \
		  $(OBJDIR)/net/ipv4.o \
		  $(OBJDIR)/net/icmp.o \
		  $(OBJDIR)/net/udp.o \
			$(OBJDIR)/net/tcp.o \
			$(OBJDIR)/filesystem/msdospart.o \
			$(OBJDIR)/filesystem/fat.o \
      $(OBJDIR)/kernel.o

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

swiftos.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

swiftos.iso: swiftos.bin
	mkdir $(ISODIR)
	mkdir $(ISODIR)/boot
	mkdir $(ISODIR)/boot/grub
	cp $< iso/boot/
	echo 'set timeout=1' >> iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "Start SwiftOS" {' >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/swiftos.bin' >> iso/boot/grub/grub.cfg
	echo '    boot' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

run-qemu: swiftos.iso
	qemu-system-i386 swiftos.iso -device pcnet,netdev=net0 -netdev user,id=net0

run: swiftos.iso
	(killall VirtualBox && sleep 1) || true
	VBoxManage startvm 'SwiftOS VirtualBox VM' &

install: swiftos.bin
	sudo cp $< /boot/swiftos.bin

.PHONY: clean
clean:
	rm -f $(objects)

setup:
	mkdir $(OBJDIR)
