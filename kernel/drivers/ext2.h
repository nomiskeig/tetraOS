#pragma once

// Implemented according to https://www.nongnu.org/ext2-doc/ext2.html
#include "kernel/libk/kstdio.h"
#include <kernel/drivers/virtio/blk.h>
#include <tlib/stdint.h>
class EXT2SuperBlock {
public:
    uint32_t s_inodes_count;
    uint32_t s_blocks_count;
    uint32_t s_r_blocks_count;
    uint32_t s_free_blocks_count;
    uint32_t s_free_inodes_count;
    uint32_t s_first_data_block;
    uint32_t s_log_block_size;
    uint32_t s_log_frag_size;
    uint32_t s_blocks_per_group;
    uint32_t s_frags_per_group;
    uint32_t s_inodes_per_group;
    uint32_t s_mtime;
    uint32_t s_wtime;
    uint16_t s_mnt_count;
    uint16_t s_max_mnt_count;
    uint16_t s_magic;
    uint16_t s_state;
    uint16_t s_errors;
    uint16_t s_minor_rev_level;
    uint32_t s_lastcheck;
    uint32_t s_ckeckinterval;
    uint32_t s_creater_os;
    uint32_t s_rev_level;
    uint16_t s_def_resuid;
    uint16_t s_def_resgid;
    // -- EXT2_DYNAMIC_REV Specific --
    uint32_t s_first_ino;
    uint16_t s_inode_size;
    // TODO: add the rest of the fields
};

class EXT2BlockGroupDescriptor {
public:
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dir_count;
    uint16_t bg_pd;
    uint8_t bg_reserved[12];
};

class EXT2Inode {
public:
    uint16_t i_mode;
    uint16_t i_uid;
    uint32_t i_size;
    uint32_t i_atime;
    uint32_t i_ctime;
    uint32_t i_mtime;
    uint32_t i_dtime;
    uint16_t i_gid;
    uint16_t i_links_count;
    uint32_t i_blocks;
    uint32_t i_flags;
    uint32_t i_osd1;
    uint32_t i_block[15];
    uint32_t i_generation;
    uint32_t i_file_acl;
    uint32_t i_dir_acl;
    uint32_t i_faddr;
    uint8_t i_osd2[12];
    uint8_t i_unused[128];
};

struct EXT2LinkedListDirectory {
    uint32_t inode;
    uint16_t rec_len;
    uint8_t name_len;
    uint8_t file_type;
    char name[255];
    bool matches(const char *other_name);
};

class EXT2 {
private:
    VirtIOBlockDevice *block_device;
    EXT2SuperBlock *super_block;
    uint32_t block_size;
    uint32_t inodes_per_block;
    EXT2BlockGroupDescriptor **group_descs;
    int get_inode_in_dir(EXT2Inode *dir, const char *name);
    int get_inode_from_dir(EXT2Inode *dir, const char *path);

public:
    EXT2(VirtIOBlockDevice *block_device);
    static EXT2 * get_instance();
    size_t get_file_size(const char *path);
    int read_file(const char *path, size_t size, char *data);
};
size_t get_file_size(const char *path);
int read_file(const char *path, size_t size, char *data);
