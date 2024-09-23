
#include "./console.h"
#include "kernel/drivers/virtio/virtio.h"
#include <kernel/libk/kstdio.h>

static VirtIOConsoleDevice *console_device;
void VirtIOConsoleDevice::set_registers(VirtIODeviceRegisters *registers) {
    this->registers = registers;
}
void VirtIOConsoleDevice::set_config(VirtIOConsoleDeviceConfig *config) {
    this->config = config;
}

// TODO: this should probalby use volatile reads/writes
int VirtIOConsoleDevice::init() {
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

    this->registers->driver_features_sel = 0x0;
    this->registers->driver_features = device_features;

    // 5. Set the FEATURES_OK status bit. The driver MUST NOT accept new feature
    // bits after this step.
    this->registers->status |= 0x8;

    // 6. Re-read device status to ensure the FEATURES_OK bit is still set:
    // otherwise, the device does not support our subset of features and the
    if ((this->registers->status & 0x8) == 0x0) {
        log(LogLevel::ERROR, "Error in console device initialization: Device "
                             "does not support the selected features");
        return -1;
    }

    // 7. Perform device-specific setup, including discovery of virtqueues for
    // the device, optional per-bus setup,
    // reading and possibly writing the device’s virtio configuration space, and
    // population of virtqueues

    // Configure the virtqueues
    // See 4.2.3.2
    for (int i = 0; i < 4; i++) {

        // 7.1. Select the queue writing its index (first queue is 0) to
        // QueueSel
        this->registers->queue_sel = i;

        // 7.2. Check if the queue is not already in use: read QueueReady, and
        // expect a returned value of zero (0x0).
        if (this->registers->queue_ready != 0x0) {
            log(LogLevel::ERROR,
                "VirtQueue for console device is already in use.");
            return -1;
        }
        // 7.3. Read maximum queue size (number of elements) from QueueNumMax.
        // If the returned value is zero
        //(0x0) the queue is not available.
        uint32_t queue_num_max = this->registers->queue_num_max;
        if (queue_num_max == 0x0) {
            log(LogLevel::ERROR,
                "VirtQueue for console device is not available.");
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
        switch (i) {
        case 0:
            this->receiveq = queue;
            break;
        case 1:
            this->transmitq = queue;
            break;
        case 2:
            this->control_receiveq = queue;
            break;
        case 3:
            this->control_transmitq = queue;
        }

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
        memory_barrier();
        // 7.7. Write 0x1 to QueueReady.
        this->registers->queue_ready = 0x1;
    }

    // notfiy the device about the regions
    // 8. Set the DRIVER_OK status bit. At this point the device is “live”.
    this->registers->status |= 0x4;

    return 0;
}

VirtIOConsoleDevice *get_console_device() {
    // TODO: return the same pointer if called the second time
    for (uint64_t address = VIRTIO_START;
         address < VIRTIO_START + VIRTIO_AMOUNT * VIRTIO_SIZE;
         address += VIRTIO_SIZE) {
        if (*(uint32_t *)address == 0x74726976 &&
            *(((uint32_t *)address) + 1) == 0x3) {
            if (*((uint32_t *)(address + 0x008)) == 0x2) {

                VirtIOConsoleDevice *device =
                    (VirtIOConsoleDevice *)kalloc(sizeof(VirtIOConsoleDevice));
                console_device = device;
                console_device->set_registers((VirtIODeviceRegisters *)address);
                console_device->set_config(
                    (VirtIOConsoleDeviceConfig *)(address + 0x100));
                return console_device;
            }
        }
    }
    return 0x0;
};
