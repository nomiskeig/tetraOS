#include <kernel/drivers/ext2.h>
#include <kernel/drivers/virtio/blk.h>
#include <kernel/libk/kstdio.h>

static EXT2 *instance;
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
    this->group_descs = new EXT2BlockGroupDescriptor *[8];
    this->block_size = 1024 << super_block->s_log_block_size;
    for (size_t i = 0; i < amount_groups; i++) {
        this->group_descs[i] = new EXT2BlockGroupDescriptor();
    }
    for (size_t i = 0; i < amount_groups; i++) {

        this->block_device->read(
            this->block_size * 1 + i * sizeof(EXT2BlockGroupDescriptor),
            sizeof(EXT2BlockGroupDescriptor), (char *)this->group_descs[i]);
    }
    this->inodes_per_block = this->block_size / this->super_block->s_inode_size;
    instance = this;
}
/*
 * Replaces the provided inode with the contents of the searched inode
 * Path is relative to inode
 * If there is a path f1/f2 and the inode of f1 is passed, then name has to be
 * f2
 * */
int EXT2::get_inode_in_dir(EXT2Inode *dir, const char *name) {
    log(LogLevel::FS, "Getting inode of dir %s in dir %s", name);
    if ((dir->i_mode & 0x4000) == 0x0) {
        log(LogLevel::ERROR,
            "get_inode_in_dir can only be called on directories, you called it "
            "on %s",
            name);
        return -1;
    }
    // assume that start_dir is already read from disc
    // TODO: free this we do not need it anymore as the inode is copied over (i
    // think)
    char *data = (char *)kalloc(this->block_size);
    // TODO: there might be more data in the blocks after the first one...
    this->block_device->read(this->block_size * dir->i_block[0],
                             this->block_size, data);
    uint64_t current_offset = 0;
    EXT2LinkedListDirectory *dir_entry =
        (EXT2LinkedListDirectory *)(data + current_offset);
    size_t counter = 0;
    while (!dir_entry->matches(name) && counter < 10) {
        current_offset += dir_entry->rec_len;
        dir_entry = (EXT2LinkedListDirectory *)(data + current_offset);
        counter++;
    }
    if (counter == 10) {
        log(LogLevel::ERROR, "Could not find the file %s.", name);
        return -1;
    }
    // TODO: this breaks if entry cannot be found


    uint32_t desc_index =
        (dir_entry->inode - 1) / this->super_block->s_inodes_per_group;
    uint32_t index =
        (dir_entry->inode - 1) % this->super_block->s_inodes_per_group;
    uint32_t containing_block = (index * sizeof(EXT2Inode)) / this->block_size;
    printf("containing block : %i\n", containing_block);
    this->block_device->read(
        this->block_size *
        // TODO: this is missing offset but i could not figure out how to calculate it
        // Should replace the + 0 below
                (group_descs[desc_index]->bg_inode_table + 0) +
            sizeof(EXT2Inode) * index,
        sizeof(EXT2Inode), (char *)dir);
    return 0;
}

size_t EXT2::get_file_size(const char *name) {
    EXT2Inode *inode = new EXT2Inode();
    this->block_device->read(this->block_size * group_descs[0]->bg_inode_table +
                                 sizeof(EXT2Inode) * 1,
                             sizeof(EXT2Inode), (char *)inode);
    get_inode_from_dir(inode, name);
    // TODO: free inode
    return inode->i_size;
}

int EXT2::read_file(const char *name, size_t size, char *data) {
    EXT2Inode *inode = new EXT2Inode();
    this->block_device->read(this->block_size * group_descs[0]->bg_inode_table +
                                 sizeof(EXT2Inode) * 1,
                             sizeof(EXT2Inode), (char *)inode);

    get_inode_from_dir(inode, name);

    this->block_device->read(this->block_size * inode->i_block[0], size, data);
    delete inode;
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

/**
 * Starts at dir and searches the path for the inode of the file the path points
 * to.
 */
int EXT2::get_inode_from_dir(EXT2Inode *dir, const char *path) {
    log(LogLevel::FS, "Getting inode of file: %s", path);

    size_t current = 0;
    size_t size = 0;
    while (path[current] != '\0') {
        if (path[current] == '/') {
            current++;
            // found a folder
            char *folder_name = (char *)kalloc(sizeof(char) * (size + 1));
            for (size_t i = 0; i < size; i++) {
                folder_name[i] = path[current - size + i - 1];
            }
            folder_name[size] = '\0';
            int res = this->get_inode_in_dir(dir, folder_name);
            if (res < 0) {
                return res;
            }
            size = 0;
        }
        current++;
        size++;
    }

    // only the file is left

    char *file_name = (char *)kalloc(sizeof(char) * (size + 1));
    for (size_t i = 0; i < size; i++) {
        file_name[i] = path[current - size + i];
    }
    file_name[size] = '\0';
    int res = this->get_inode_in_dir(dir, file_name);
    if (res < 0) {
        return res;
    }
    return 0;
}
EXT2 *EXT2::get_instance() { return instance; }

size_t get_file_size(const char *path) {
    return EXT2::get_instance()->get_file_size(path);
}
int read_file(const char *path, size_t size, char *data) {
    return EXT2::get_instance()->read_file(path, size, data);
}
