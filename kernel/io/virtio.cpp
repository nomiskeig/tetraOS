#include "../include/drivers/virtio/virtio.h"
#include "../include/drivers/virtio/blk.h"
#include "../include/libk/kstdio.h"

static VirtIOBlockDevice *block_device;
void VirtIOBlockDevice::set_registers(VirtIODeviceRegisters *registers) {
    this->registers = registers;
}
void VirtIOBlockDevice::set_config(VirtIOBlockDeviceConfig *config) {
    this->config = config;
}
// TODO: this should probalby use volatile reads/writes
int VirtIOBlockDevice::init() {
    // See 3.1 of virtio spec
    // 1. Reset the device.
    this->registers->status = 0x0;

    // 2. Set the ACKNOWLEDGE status bit: the guest OS has noticed the device.
    this->registers->status = 0x1;

    // 3. Set the DRIVER status bit: the guest OS knows how to drive the device.
    this->registers->status |= 0x2;

    // 4. Read device feature bits, and write the subset of feature bits
    // understood by the OS and driver to the device. During this step the
    // driver MAY read (but MUST NOT write) the device-specific configuration
    // fields to check that it can support the device before accepting it.
    this->registers->device_features_sel = 0x0;
    uint32_t device_features = this->registers->device_features;
    this->registers->device_features_sel = 0x1;
    uint32_t device_features2 = this->registers->device_features;
    printf("device features: 0x%x\n", device_features);
    printf("device features 2: 0x%x\n", device_features2);
    printf("VIRTIO_F_NOTIFICATION_DATA: 0x%x\n", device_features2 >> 6 & 0x1);

    this->registers->driver_features_sel = 0x0;
    this->registers->driver_features = device_features;

    // 5. Set the FEATURES_OK status bit. The driver MUST NOT accept new feature
    // bits after this step.
    this->registers->status |= 0x8;

    // 6. Re-read device status to ensure the FEATURES_OK bit is still set:
    // otherwise, the device does not support our subset of features and the
    if ((this->registers->status & 0x8) == 0x0) {
        log(LogLevel::ERROR, "Error in block device initialization: Device "
                             "does not support the selected features");
        return -1;
    }

    // 7. Perform device-specific setup, including discovery of virtqueues for
    // the device, optional per-bus setup,
    // reading and possibly writing the device’s virtio configuration space, and
    // population of virtqueues

    // Configure the virtqueues
    // See 4.2.3.2
    // 7.1. Select the queue writing its index (first queue is 0) to QueueSel
    this->registers->queue_sel = 0x0;

    // 7.2. Check if the queue is not already in use: read QueueReady, and
    // expect a returned value of zero (0x0).
    if (this->registers->queue_ready != 0x0) {
        log(LogLevel::ERROR, "VirtQueue for block device is already in use.");
        return -1;
    }
    // 7.3. Read maximum queue size (number of elements) from QueueNumMax. If
    // the returned value is zero
    //(0x0) the queue is not available.
    uint32_t queue_num_max = this->registers->queue_num_max;
    if (queue_num_max == 0x0) {
        log(LogLevel::ERROR, "VirtQueue for block device is not available.");
        return -1;
    }
    if (queue_num_max < VIRTIO_QUEUE_SIZE) {
        log(LogLevel::ERROR,
            "VirtQueue does not support enough queue elements.");
        return -1;
    }
    // 7.4. Allocate and zero the queue memory, making sure the memory is
    // physically contiguous.
    VirtQueue *queue = new VirtQueue();
    this->queue = queue;
    // 7.5. Notify the device about the queue size by writing the size to
    // QueueNum.
    this->registers->queue_num = VIRTIO_QUEUE_SIZE;

    // 7.6. Write physical addresses of the queue’s
    // Descriptor Area, Driver Area and Device Area to
    // (respectively)
    // the QueueDescLow/QueueDescHigh, QueueDriverLow/QueueDriverHigh and
    // QueueDeviceLow/QueueDeviceHigh register pairs.
    this->registers->queue_desc =
        (uint64_t)get_physical_address_of_virtual_address(&queue->desc);
    this->registers->queue_driver =
        (uint64_t)get_physical_address_of_virtual_address(&queue->avail);
    this->registers->queue_device =
        (uint64_t)get_physical_address_of_virtual_address(&queue->used);
    printf("Desc address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(&queue->desc));
    memory_barrier();
    printf("Driver address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(&queue->avail));
    printf("Device address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(&queue->used));
    // 7.7. Write 0x1 to QueueReady.
    this->registers->queue_ready = 0x1;

    uint64_t device_size = this->config->capacity;
    uint16_t amount_queues = this->config->num_queues;
    printf("capacity: 0x%x\n", device_size);
    printf("blk size: 0x%x\n", this->config->blk_size);
    printf("amount queues: %i\n", amount_queues);
    printf("queue size: 0x%x\n", this->registers->queue_num_max);

    // notfiy the device about the regions
    // 8. Set the DRIVER_OK status bit. At this point the device is “live”.
    this->registers->status |= 0x4;

    return 0;
}
/*
 * Reads length bytes from the offset. Offset is given in bytes. Result is
 * written into buffer.
 */
void VirtIOBlockDevice::read(uint64_t offset, uint64_t length, char *buffer) {
    VirtIOBlkRequest *request = new VirtIOBlkRequest(0, 2);
    request->status.status = 0x12;
    printf("gets here, 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(request));

    void* volatile data_pointer = kalloc(512);
    this->queue->desc[0].addr =
        (uint64_t)get_physical_address_of_virtual_address(&request->header);
    printf("desc0 address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(&request->header));
    this->queue->desc[0].len = sizeof(VirtIOBlkRequestHeader);
    this->queue->desc[0].flags = VIRTQ_DESC_F_NEXT;
    this->queue->desc[0].next = 1;
    this->queue->desc[1].addr =
        (uint64_t)get_physical_address_of_virtual_address(data_pointer);
    printf("desc1 virtual address: 0x%x\n", &request->data);
    printf("desc1 address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(data_pointer));
    this->queue->desc[1].len = 512;
    this->queue->desc[1].next = 2;
    this->queue->desc[1].flags = VIRTQ_DESC_F_NEXT | VIRTQ_DESC_F_WRITE;
    this->queue->desc[2].addr =
        (uint64_t)get_physical_address_of_virtual_address(&request->status);
    printf("desc2 virtual address: 0x%x\n", &request->status);
    printf("desc2 address: 0x%x\n",
           (uint64_t)get_physical_address_of_virtual_address(&request->status));
    this->queue->desc[2].len = sizeof(VirtIOBlkRequestStatus);
    this->queue->desc[2].flags = VIRTQ_DESC_F_WRITE;
    this->queue->avail.ring[this->queue->avail.idx % VIRTIO_QUEUE_SIZE] = 0x0;
    memory_barrier();
    this->queue->avail.idx += 1;
    memory_barrier();
    this->registers->queue_notify = 0x0;
    printf("read\n");
    // while (true) {
    printf("status: 0x%x\n", request->status.status);
    for (size_t i = 0; i < 512/8; i++) {
        printf("data at 0x%x: 0x%x\n", (uint64_t*)data_pointer + i, *((uint64_t*)(data_pointer) + i));
    }
    //}
}
VirtIOBlockDevice *get_block_device() {
    if (block_device != 0x0) {
        return block_device;
    }
    for (uint64_t address = VIRTIO_START;
         address < VIRTIO_START + VIRTIO_AMOUNT * VIRTIO_SIZE;
         address += VIRTIO_SIZE) {
        if (*(uint32_t *)address == 0x74726976 &&
            *(((uint32_t *)address) + 1) == 0x2) {
            if (*((uint32_t *)(address + 0x008)) == 0x2) {

                printf("found block device at 0x%x\n", address);
                VirtIOBlockDevice *device =
                    (VirtIOBlockDevice *)kalloc(sizeof(VirtIOBlockDevice));
                block_device = device;
                block_device->set_registers((VirtIODeviceRegisters *)address);
                block_device->set_config(
                    (VirtIOBlockDeviceConfig *)(address + 0x100));
                return block_device;
            }
            printf("found at 0x%x\n", address);
        }
    }
    return 0x0;
};

VirtIOBlkRequest::VirtIOBlkRequest(uint32_t type, uint32_t sector) {
    this->header.type = type;
    this->header.sector = sector;
}
