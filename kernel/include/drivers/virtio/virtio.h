
#ifndef VIRTIODEVICE
#define VIRTIODEVICE
#include "../../memory.h"

#define VIRTIO_START 0x10001000 + VIRTUAL_OFFSET
#define VIRTIO_AMOUNT 8
#define VIRTIO_SIZE 0x1000
#define VIRTIO_QUEUE_SIZE 256

#include <cstdint>
enum VirtIODeviceID {
    NETWORK_CARD = 1,
    BLOCK_DEVICE = 2,
};
class VirtIODeviceRegisters {
public:
    uint32_t magic_value;
    uint32_t version;
    uint32_t device_id;
    uint32_t vendor_id;
    uint32_t device_features;
    uint32_t device_features_sel;
    uint32_t spacing1[2];
    uint32_t driver_features;
    uint32_t driver_features_sel;
    uint32_t spacing2[2];
    uint32_t queue_sel;
    uint32_t queue_num_max;
    uint32_t queue_num;
    uint32_t spacing3[2];
    uint32_t queue_ready;
    uint32_t spacing4[2];
    uint32_t queue_notify;
    uint32_t spacing5[3];
    uint32_t interrupt_status;
    uint32_t interrupt_ack;
    uint32_t spacing6[2];
    uint32_t status;
    uint32_t spacing7[3];
    uint64_t queue_desc;
    uint32_t spacing8[2];
    uint64_t queue_driver;
    uint32_t spacing9[2];
    uint64_t queue_device;
    uint32_t spacing10;
    uint32_t shm_sel;
    uint64_t shm_len;
    uint64_t shm_base;
    uint32_t queue_reset;
    uint32_t spacing11[14];
    uint32_t config_generation;
};
/* This marks a buffer as continuing via the next field. */
#define VIRTQ_DESC_F_NEXT 1
/* This marks a buffer as device write-only (otherwise device read-only). */
#define VIRTQ_DESC_F_WRITE 2
/* This means the buffer contains a list of buffer descriptors. */
#define VIRTQ_DESC_F_INDIRECT 4

class VirtQueueDesc {
public:
    /* Address (guest-physical). */
    uint64_t addr;
    /* Length. */
    uint32_t len;
    /* The flags as indicated above. */
    uint16_t flags;
    /* Next field if flags & NEXT */
    uint16_t next;
};
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1
class VirtQueueAvail {
public:
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[VIRTIO_QUEUE_SIZE];
    uint16_t used_event; /* Only if VIRTIO_F_EVENT_IDX */
};
class VirtQueueUsedElem {
public:
    /* Index of start of used descriptor chain. */
    uint32_t id;
    /*
     * The number of bytes written into the device writable portion of
     * the buffer described by the descriptor chain.
     */
    uint32_t len;
};
class VirtQueueUsed {
public:
#define VIRTQ_USED_F_NO_NOTIFY 1
    uint16_t flags;
    uint16_t idx;
    VirtQueueUsedElem ring[VIRTIO_QUEUE_SIZE];
    uint16_t avail_event; /* Only if VIRTIO_F_EVENT_IDX */
};
class VirtQueue {
public:
    VirtQueueDesc desc[VIRTIO_QUEUE_SIZE];
    VirtQueueAvail avail;
    VirtQueueUsed used;
};
#endif
