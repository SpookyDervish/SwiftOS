#include <filesystem/fat.h>
#include <filesystem/msdospart.h>

using namespace swiftos;
using namespace swiftos::filesystem;
using namespace swiftos::drivers;
using namespace swiftos::common;

void printf(char *);
void printfHex(uint8_t);

void MSDOSPartitionTable::ReadPartitions(swiftos::drivers::AdvancedTechnologyAttachment *hd)
{
    MasterBootRecord mbr;
    // TODO: Fix this
    // hd->Read28(0, (uint8_t *)&mbr, sizeof(MasterBootRecord));

    // Debugging stuffs
    /*
    printf("MBR: ");
    for (int i = 0x1BE; i <= 0x01FF; i++)
    {
        printfHex(((uint8_t *)&mbr)[i]);
        printf(" ");
    }
    printf("\n");
  */

    if (mbr.magicnumber != 0xAA55)
    {
        printf("Illegal MBR!");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        if (mbr.primaryPartition[i].partition_id == 0)
            continue;

        printf(" Partition ");
        printfHex(i & 0xFF);

        if (mbr.primaryPartition[i].bootable == 0x80)
            printf(" bootable. Type");
        else
            printf(" not bootable. Type");

        printfHex(mbr.primaryPartition[i].partition_id);

        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
}
