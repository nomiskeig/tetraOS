
#include "kernel/drivers/ext2.h"


class VFS {

private:
    EXT2Inode* open_files[10];
    // 0-2 are stdout,stdin, stderr
    size_t next_descriptor = 3;
    EXT2* fs;

public:
    int open_file(const char* file_name);
    VFS();
    int get_next_descriptor();
    static VFS* get_instance();


};



