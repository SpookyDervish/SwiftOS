#include <filesystem/fat.h>

using namespace swiftos;
using namespace swiftos::common;
using namespace swiftos::filesystem;
using namespace swiftos::drivers;

void printf(char *);
void printfHex(uint8_t);

void swiftos::filesystem::ReadBiosBlock(swiftos::drivers::AdvancedTechnologyAttachment *hd,
                                        common::uint32_t partitionOffset)
{
    BiosParameterBlock32 bpb;
    // TODO: Fix this too
    hd->Read28(partitionOffset, (uint8_t *)&bpb, sizeof(BiosParameterBlock32));

    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb.fatCopies;

    uint32_t rootStart = dataStart + bpb.sectorsPerCluster * (bpb.rootCluster - 2);

    DirectoryEntryFat32 dirent[16];
    // TODO Fix this tooooooo
    hd->Read28(rootStart, (uint8_t *)&dirent[0], 16 * sizeof(DirectoryEntryFat32));

    for (int i = 0; i < 16; i++)
    {
        if (dirent[i].name[0] == 0x00)
            break;

        if ((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        // This is a bit hackish, but since a file can't have a name with `/n`
        // and printf expects a `\n`, we have to do this
        char *foo = "        \n";
        for (int j = 0; j < 8; j++)
            foo[j] = dirent[i].name[j];
        printf(foo);

        if ((dirent[i].attributes & 0x10) == 0x10) // This is a directory
            continue;

        uint32_t fileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16 | ((uint32_t)dirent[i].firstClusterLow);
        uint32_t fileSector = dataStart + bpb.sectorsPerCluster * (fileCluster - 2);

        uint8_t buffer[512];
        hd->Read28(fileSector, buffer, 512);
        buffer[dirent[i].size] = '\0';
        printf((char *)buffer);
    }
}
