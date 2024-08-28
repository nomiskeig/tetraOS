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
    int amount_groups = this->super_block->s_blocks_count /
                        this->super_block->s_blocks_per_group;
    this->group_descs = new EXT2BlockGroupDescriptor*[8];
    this->block_size = 1024 << super_block->s_log_block_size;
    for (size_t i = 0; i < amount_groups; i++) {

        this->group_descs[i] = new EXT2BlockGroupDescriptor();
    }
    for (size_t i = 0; i < amount_groups; i++) {

    this->block_device->read(this->block_size * 1 + i*sizeof(EXT2BlockGroupDescriptor),
                             sizeof(EXT2BlockGroupDescriptor), (char*)this->group_descs[i]);
    }
    this->inodes_per_block = this->block_size / this->super_block->s_inode_size;

}
int EXT2::get_inode(EXT2Inode *start_dir, EXT2BlockGroupDescriptor *desc,
                    const char *name) {
    log(LogLevel::FS, "Getting inode of dir %s", name);
    if ((start_dir->i_mode & 0x4000) == 0x0) {
        log(LogLevel::ERROR, "get_inode can only be called on directories");
         return -1;
    }
    // assume that start_dir is already read from disc
    // TODO: free this we do not need it anymore as the inode is copied over (i
    // think)
    char *data = (char *)kalloc(this->block_size);
    // TODO: there might be more data in the blocks after the first one...
    this->block_device->read(this->block_size * start_dir->i_block[0],
                             this->block_size, data);
    uint64_t current_offset = 0;
    EXT2LinkedListDirectory *dir_entry =
        (EXT2LinkedListDirectory *)(data + current_offset);
    while (!dir_entry->matches(name)) {
        current_offset += dir_entry->rec_len;
        dir_entry = (EXT2LinkedListDirectory *)(data + current_offset);
    }
    // TODO: this breaks if entry cannot be found

    // TODO: this does not work, it calculates the offset wrong i think

    uint32_t desc_index =  (dir_entry->inode-1)/this->super_block->s_inodes_per_group;
    this->block_device->read(
        this->block_size *
        // TODO: the commented out code has to calculate the index of the block on which the inode were looking for is stored
                (group_descs[desc_index]->bg_inode_table /* + (dir_entry->inode-1) / this->inodes_per_block*/) +
            sizeof(EXT2Inode) * ((dir_entry->inode -1) % this->inodes_per_block),
        sizeof(EXT2Inode), (char *)start_dir);
    return 0;
}

int EXT2::read_file(const char *name) {
    log(LogLevel::FS, "Reading file: %s", name);

    EXT2Inode *current_inode = new EXT2Inode();
    this->block_device->read(this->block_size * group_descs[0]->bg_inode_table +
                                 sizeof(EXT2Inode) * 1,
                             sizeof(EXT2Inode), (char *)current_inode);
    size_t current = 0;
    size_t size = 0;
    while (name[current] != '\0') {
        if (name[current] == '/') {
            current++;
            // found a folder
            char *folder_name = (char *)kalloc(sizeof(char) * (size + 1));
            for (size_t i = 0; i < size; i++) {
                folder_name[i] = name[current - size + i - 1];
            }
            folder_name[size] = '\0';
            this->get_inode(current_inode, group_descs[0], folder_name);
            size = 0;
        }
        current++;
        size++;
    }

    // only the file is left

    EXT2Inode *file_inode = new EXT2Inode();
    char *file_name = (char *)kalloc(sizeof(char) * (size + 1));
    for (size_t i = 0; i < size; i++) {
        file_name[i] = name[current - size + i];
    }
    file_name[size] = '\0';
    this->get_inode(current_inode, group_descs[0], file_name);
    char *data = (char *)kalloc(this->block_size);

    this->block_device->read(this->block_size * current_inode->i_block[0],
                             this->block_size, data);
    for (size_t i = 0; i < 100; i++) {
        printf("%c", *(data + i));
    }
    return 0;
}
bool EXT2LinkedListDirectory::matches(const char *other_name) {
    size_t other_len = 0;
    while (other_name[other_len] != '\0') {
        other_len++;
    };
    if (other_len != this->name_len) {
        return false;
    }
    for (size_t i = 0; i < other_len; i++) {
        if (other_name[i] != this->name[i]) {
            return false;
        }
    }
    return true;
}
