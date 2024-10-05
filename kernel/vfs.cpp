#include "vfs.h"

static VFS *instance;
int VFS::open_file(const char *file_name) {
    EXT2 *ext2 = EXT2::get_instance();
    EXT2Inode *inode = new EXT2Inode();
    ext2->get_root_inode(inode);
    ext2->get_inode_from_dir(inode, file_name);
    size_t next_file_descriptor = this->get_next_descriptor();
    //this->open_files[next_descriptor] = inode;
    //this->open_files[3] = inode;
    return 3;
    
    //return next_file_descriptor;
}

int VFS::get_next_descriptor() { return this->next_descriptor++; }

VFS *VFS::get_instance() { return instance; }

VFS::VFS() {
    this->next_descriptor = 3;
    instance = this;

}
