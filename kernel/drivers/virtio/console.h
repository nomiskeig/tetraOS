#ifndef CONSOLE_H
#define CONSOLE_H
#include <kernel/drivers/virtio/virtio.h>
#include <tlib/stdint.h>
class VirtIOConsoleDeviceConfig {
public:
    uint16_t cols;
    uint16_t rows;
    uint32_t max_nr_ports;
    uint32_t emerg_wr;
};

class VirtIOConsoleDevice {
    VirtIODeviceRegisters *registers;
    VirtIOConsoleDeviceConfig* config;
    VirtQueue *receiveq;
    VirtQueue* transmitq;
    VirtQueue* control_receiveq;
    VirtQueue* control_transmitq;
public:
    int init();
    void set_registers(VirtIODeviceRegisters* registers);
    void set_config(VirtIOConsoleDeviceConfig* registers);

};

VirtIOConsoleDevice *get_console_device();

#endif
