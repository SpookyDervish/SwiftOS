#ifndef __SWIFTOS__FILESYSTEM__FAT_H
#define __SWIFTOS__FILESYSTEM__FAT_H

#include <common/types.h>
#include <drivers/ata.h>

namespace swiftos
{
namespace filesystem
{
struct BiosParameterBlock32
{
    common::uint8_t jump[3];           // Jump instruction
    common::uint8_t softName[3];       // Software name, FAT in our case
    common::uint16_t bytesPerSector;   // The number of bytes in a single sector
    common::uint8_t sectorsPerCluster; // The number of sectors in a single cluster
    common::uint16_t reservedSectors;  // Reserved (duh...)
    common::uint8_t
        fatCopies; // We store copies of the FAT (File Allocation Table), and this tells us how many to store
    common::uint16_t rootDirEntries;   // Number of files, zero in our case
    common::uint16_t totalSectors;     //
    common::uint8_t mediaType;         //
    common::uint16_t fatSectorCount;   // FAT16 version of the sector count, we ignore this as we're using FAT32
    common::uint16_t sectorsPerTrack;  //
    common::uint16_t headCount;        //
    common::uint32_t hiddenSectors;    //
    common::uint32_t totalSectorCount; //

    common::uint32_t tableSize;      //
    common::uint16_t extFlags;       //
    common::uint16_t fatVersion;     //
    common::uint32_t rootCluster;    // One of the very relevant things
    common::uint16_t fatInfo;        //
    common::uint16_t backupSector;   // Redundency sector
    common::uint8_t reserved0[12];   // Reserved ones (duh... again...)
    common::uint8_t driveNumber;     //
    common::uint8_t reserved;        //
    common::uint8_t bootSignature;   //
    common::uint32_t volumeId;       //
    common::uint8_t volumeLabel[11]; //
    common::uint8_t fatTypeLabel[8]; //

} __attribute__((packed));

struct DirectoryEntryFat32
{
    common::uint8_t name[8];    //
    common::uint8_t ext[3];     //
    common::uint8_t attributes; //
    common::uint8_t reserved;   //
    common::uint8_t cTimeTenth; //
    common::uint16_t cTime;     // Creation time
    common::uint16_t cDate;     // Creation date
    common::uint16_t aTime;     // Access time

    common::uint16_t firstClusterHi; //

    common::uint16_t wTime;           // Last write time
    common::uint16_t wDate;           // Last write date
    common::uint16_t firstClusterLow; //
    common::uint32_t size;            //
} __attribute__((packed));

void ReadBiosBlock(swiftos::drivers::AdvancedTechnologyAttachment *hd, common::uint32_t partitionOffset);
} // namespace filesystem
} // namespace swiftos

#endif // !__SWIFTOS__FILESYSTEM__FAT_H
