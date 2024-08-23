#ifndef BLKH
#define BLKH
#include "virtio.h"
class VirtIOBlockDeviceConfig {
public:
    uint64_t capacity;
    uint32_t size_max;
    uint32_t seg_max;
    struct virtio_blk_geometry {
        uint16_t cylinders;
        uint8_t heads;
        uint8_t sectors;
    } geometry;
    uint32_t blk_size;
    struct virtio_blk_topology {
        // # of logical blocks per physical block (log2)
        uint8_t physical_block_exp;
        // offset of first aligned logical block
        uint8_t alignment_offset;
        // suggested minimum I/O size in blocks
        uint16_t min_io_size;
        // optimal (suggested maximum) I/O size in blocks
        uint32_t opt_io_size;
    } topology;
    uint8_t writeback;
    uint8_t unused0;
    uint16_t num_queues;
    uint32_t max_discard_sectors;
    uint32_t max_discard_seg;
    uint32_t discard_sector_alignment;
    uint32_t max_write_zeroes_sectors;
    uint32_t max_write_zeroes_seg;
    uint8_t write_zeroes_may_unmap;
    uint8_t unused1[3];
    uint32_t max_secure_erase_sectors;
    uint32_t max_secure_erase_seg;
    uint32_t secure_erase_sector_alignment;
};
class VirtIOBlockDevice {
private:
    VirtIODeviceRegisters *registers;
    VirtIOBlockDeviceConfig *config;
    VirtQueue *queue;
    int basic_op(uint64_t sector, uint64_t length, char* buffer, uint8_t type);
    int write_blocks(uint64_t sector, uint64_t length, char *buffer);
    int read_blocks(uint64_t sector, uint64_t length, char *buffer);
public:
    int init();
    void set_registers(VirtIODeviceRegisters *registers);
    void set_config(VirtIOBlockDeviceConfig *config);
    int write(uint64_t offset, uint64_t length, char* buffer);
    int read(uint64_t offset, uint64_t length, char* buffer);
};

VirtIOBlockDevice *get_block_device();

class VirtIOBlkRequestHeader {
public:
    uint32_t type;
    uint32_t reserved;
    uint64_t sector;

};
class VirtIOBlkRequestData {
public:
    volatile uint8_t data[512];

};
class VirtIOBlkRequestStatus {
public:
    volatile uint8_t status;

};
class VirtIOBlkRequest {
public:
    VirtIOBlkRequestHeader header;
    VirtIOBlkRequestData data;
    VirtIOBlkRequestStatus status;
    VirtIOBlkRequest(uint32_t type, uint32_t sector);
};
#endif
