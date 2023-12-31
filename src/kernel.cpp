#include <common/types.h>
#include <drivers/amd_am79c973.h>
#include <drivers/ata.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <filesystem/msdospart.h>
#include <gdt.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <memorymanagement.h>
#include <multitasking.h>
#include <net/arp.h>
#include <net/etherframe.h>
#include <net/icmp.h>
#include <net/ipv4.h>
#include <net/tcp.h>
#include <net/udp.h>
#include <syscalls.h>

#define GRAPHICSMODE

using namespace swiftos;
using namespace swiftos::common;
using namespace swiftos::drivers;
using namespace swiftos::filesystem;
using namespace swiftos::hardwarecommunication;
using namespace swiftos::gui;
using namespace swiftos::net;

static uint16_t *VideoMemory = (uint16_t *)0xb8000;
static uint8_t x = 0, y = 0;

void clear()
{
    for (y = 0; y < 25; y++)
    {
        for (x = 0; x < 80; x++)
        {
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
        }
    }

    x = 0;
    y = 0;
}

void printf(char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            x = 0;
            y++;
            break;

        case '\r':
            x = 0;

        default:
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }

        if (x >= 80)
        {
            y++;
            x = 0;
        }

        if (y >= 25)
        {
            y--;
            x = 0;
            clear();
        }
    }
}

void printfHex(uint8_t key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
void printfHex16(uint16_t key)
{
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}
void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex(key & 0xFF);
}

void print_backspace()
{
    x--;
    printf(" ");
    x--;
}

void print_go_left()
{
    x--;
}

void print_go_right()
{
    x++;
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
  public:
    void OnKeyDown(char c)
    {
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;

  public:
    MouseToConsole()
    {
    }

    virtual void OnActivate()
    {
        uint16_t *VideoMemory = (uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 |
                                  (VideoMemory[80 * y + x] & 0x00FF);
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 |
                                  (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 |
                                  (VideoMemory[80 * y + x] & 0x00FF);
    }
};

class PrintfUDPHandler : public UserDatagramProtocolHandler
{
  public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket *socket, common::uint8_t *data,
                                           common::uint16_t size)
    {
        char *foo = " ";
        for (int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }
    }
};

class PrintfTCPHandler : public TransmissionControlProtocolHandler
{
  public:
    bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket *socket, common::uint8_t *data,
                                                  common::uint16_t size)
    {
        char *foo = " ";
        for (int i = 0; i < size; i++)
        {
            foo[0] = data[i];
            printf(foo);
        }

        if (size > 9 && data[0] == 'G' && data[1] == 'E' && data[2] == 'T' && data[3] == ' ' && data[4] == '/' &&
            data[5] == ' ' && data[6] == 'H' && data[7] == 'T' && data[8] == 'T' && data[9] == 'P')
        {
            socket->Send((uint8_t *)"HTTP/1.1 200 OK\r\nServer: MyOS\r\nContent-Type: "
                                    "text/html\r\n\r\n<html><head><title>My Operating "
                                    "System</title></head><body><b>My Operating System</b> "
                                    "http://www.AlgorithMan.de</body></html>\r\n",
                         184);
            socket->Disconnect();
        }

        return true;
    }
};

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void kernelMain(const void *multibootStructure, uint32_t magicNumber)
{
    // Clear the screen and print a boot message
    clear();
    printf("Starting SwiftOS...\n\n");

    // Instantiate the GDT and IDT
    GlobalDescriptorTable gdt;

    // Memory management
    uint32_t *memupper = (uint32_t *)(((size_t)multibootStructure) + 8);

    size_t heap = 10 * 1024 * 1024; // Pre-determined value
    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8) & 0xFF);
    printfHex((heap)&0xFF);

    void *allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8) & 0xFF);
    printfHex(((size_t)allocated) & 0xFF);
    printf("\n");

    // Multithreading
    TaskManager taskManager;
    InterruptManager interrupts(0x20, &gdt, &taskManager);

    // Syscalls
    SyscallHandler syscalls(&interrupts, 0x80);

    printf("Initializing Hardware, Stage 1\n");

#ifdef GRAPHICSMODE
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif

    DriverManager drvManager;

#ifdef GRAPHICSMODE
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);

#ifdef GRAPHICSMODE
    KeyboardDriver keyboard(&interrupts, &desktop);
#else
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
#endif
    drvManager.AddDriver(&keyboard);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

#ifdef GRAPHICSMODE
    VideoGraphicsArray vga;
#endif

    // Activate all drivers
    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();

    // Activate interrupts
    printf("Initializing Hardware, Stage 3\n");

#ifdef GRAPHICSMODE
    vga.SetMode(320, 200, 8);
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    desktop.AddChild(&win1);
    Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
    desktop.AddChild(&win2);
#endif

    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    ata0m.Identify();

    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(false, 0x1F0);
    ata0s.Identify();
    /*ata0s.Write28(0, (uint8_t *)"sus amogus", 11);
    ata0s.Flush();
    ata0s.Read28(0, 11);*/

    /*printf("\nS-ATA secondary master: ");
    AdvancedTechnologyAttachment ata1m(true, 0x170);
    ata1m.Identify();

    printf("\nS-ATA secondary slave: ");
    AdvancedTechnologyAttachment ata1s(false, 0x170);
    ata1s.Identify();*/

    printf("\n\n\n\n\n\n");
    MSDOSPartitionTable::ReadPartitions(&ata0s);

    // interrupt 15
    AdvancedTechnologyAttachment ata1m(0x170, true);
    AdvancedTechnologyAttachment ata1s(0x170, false);

    // third: 0x1E8
    // fourth: 0x168

    amd_am79c973 *eth0 = (amd_am79c973 *)(drvManager.drivers[2]);
    // IP address
    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    uint32_t ip_be = ((uint32_t)ip4 << 24) | ((uint32_t)ip3 << 16) | ((uint32_t)ip2 << 8) | (uint32_t)ip1;
    eth0->SetIPAddress(ip_be);
    EtherFrameProvider etherframe(eth0);
    AddressResolutionProtocol arp(&etherframe);

    // IP Address of the default gateway
    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24) | ((uint32_t)gip3 << 16) | ((uint32_t)gip2 << 8) | (uint32_t)gip1;

    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be =
        ((uint32_t)subnet4 << 24) | ((uint32_t)subnet3 << 16) | ((uint32_t)subnet2 << 8) | (uint32_t)subnet1;

    InternetProtocolProvider ipv4(&etherframe, &arp, gip_be, subnet_be);
    InternetControlMessageProtocol icmp(&ipv4);
    UserDatagramProtocolProvider udp(&ipv4);
    TransmissionControlProtocolProvider tcp(&ipv4);

    interrupts.Activate();
    // udpsocket->Send((uint8_t*)"Hello UDP!", 10);

    // UserDatagramProtocolSocket* udpsocket = udp.Listen(1234);
    // udp.Bind(udpsocket, &udphandler);

    while (1)
    {
#ifdef GRAPHICSMODE
        desktop.Draw(&vga);
#endif
    }
}
