#include <kernel/drivers/ext2.h>
#include <kernel/drivers/virtio/blk.h>
#include <kernel/libk/kstdio.h>
static uint32_t ext2_block_size;
static VirtIOBlockDevice* block_device ;
static EXT2SuperBlock* super_block;


void ext2_init(VirtIOBlockDevice *blk_device) {
    super_block = new EXT2SuperBlock();
    block_device = blk_device;
    block_device->read(1024, sizeof(EXT2SuperBlock), (char *)super_block);
    printf("Amount of inodes: %i", super_block->s_inodes_count);
    ext2_block_size = 1024 << super_block ->s_log_block_size; 

}


void ext2_read_file(char* name) {
    EXT2BlockGroupDescriptor* desc = new EXT2BlockGroupDescriptor();
    printf("block size: %i", ext2_block_size);

}

