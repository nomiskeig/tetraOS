#include <kernel/drivers/ext2.h>
#include <kernel/drivers/virtio/blk.h>
#include <kernel/libk/kstdio.h>

void print_entry_name(EXT2LinkedListDirectory *entry) {
    printf("Entry name: ");
    for (size_t i = 0; i < entry->name_len; i++) {
        printf("%c", entry->name[i]);
    }
    printf("\n");
}
EXT2::EXT2(VirtIOBlockDevice *block_device) {
    this->block_device = block_device;
    this->super_block = new EXT2SuperBlock();
    block_device->read(1024, sizeof(EXT2SuperBlock), (char *)super_block);
    this->block_size = 1024 << super_block->s_log_block_size;
}

void EXT2::read_file(const char *name) {
    EXT2BlockGroupDescriptor *desc = new EXT2BlockGroupDescriptor();
    this->block_device->read(this->block_size * 1,
                             sizeof(EXT2BlockGroupDescriptor), (char *)desc);
    EXT2Inode *root_inode = new EXT2Inode();
    this->block_device->read(this->block_size * desc->bg_inode_table +
                                 sizeof(EXT2Inode) * 1,
                             sizeof(EXT2Inode), (char *)root_inode);

    char *dir_block = (char *)kalloc(this->block_size);
    uint64_t current_offset = 0;
    EXT2LinkedListDirectory *dir_entry =
        (EXT2LinkedListDirectory *)(dir_block + current_offset);
    this->block_device->read(this->block_size * root_inode->i_block[0],
                             this->block_size, dir_block);
    current_offset += dir_entry->rec_len;
    dir_entry = (EXT2LinkedListDirectory *)(dir_block + current_offset);
    current_offset += dir_entry->rec_len;
    dir_entry = (EXT2LinkedListDirectory *)(dir_block + current_offset);
    current_offset += dir_entry->rec_len;
    dir_entry = (EXT2LinkedListDirectory *)(dir_block + current_offset);
    EXT2Inode *test_file_indode = new EXT2Inode();
    this->block_device->read(this->block_size * desc->bg_inode_table +
                                 (sizeof(EXT2Inode) * (dir_entry->inode -1)),
                             sizeof(EXT2Inode), (char *)test_file_indode);

    char *data = (char *)kalloc(this->block_size);

    this->block_device->read(this->block_size * test_file_indode->i_block[0],
                             this->block_size, data);
    for (size_t i = 0; i<100; i++) {
        printf("%c", *(data + i));
    }
}
