#include "../include//libk/kstdio.h"
#include <cstdint>

#include "../include/os.h"
typedef uint64_t pageframe_t;
#define FRAME_SIZE 4096
#define FRAMES_FOR_MANAGING 8
#define VIRTUAL_OFFSET  0xFFFF800000000000
extern int64_t TEXT_START;
extern int64_t TEXT_END;
extern int64_t RODATA_START;
extern int64_t RODATA_END;
extern int64_t DATA_START;
extern int64_t DATA_END;
extern int64_t BSS_START;
extern int64_t BSS_END;
extern int64_t PHYSICAL_HEAP_START;
extern int64_t PHYSICAL_HEAP_SIZE;


static uint64_t managed_frames_address_start;
static pageframe_t first_frame_address;

struct PageframeFlags {
    // 00: free
    // 01: allocated
    uint8_t flags;
};

uint32_t get_frame_number(pageframe_t frame_address) {
    return (frame_address - first_frame_address) / FRAME_SIZE;
}
void set_pageframe_free(PageframeFlags *frame) { frame->flags = 0x0; }
void set_pageframe_allocated(PageframeFlags *frame) { frame->flags = 0x1; }
bool is_free(PageframeFlags *frameFlags) { return frameFlags->flags == 0x0; }
uint64_t allign_address(uint64_t address, uint64_t num) {
    return (address + num - 1) & ~(num - 1);
}
void physical_allocator_init() {
    /* We have 4096 mb of ram, that is 2^^32 / 2^^12 = 2 ^^ 20 pages
     * One page can manage 2^^12 pages, thus we need 2^^20 / 2^^12 = 2^^8 = 256
     * pages for management
     *
     */
    uint32_t num_frames = PHYSICAL_HEAP_SIZE / FRAME_SIZE - FRAMES_FOR_MANAGING;
    printf("Memory regions:\n");
    printf("TEXT:            0x%x -> 0x%x\n", TEXT_START, TEXT_END);
    printf("RODATA:          0x%x -> 0x%x\n", RODATA_START, RODATA_END);
    printf("DATA:            0x%x -> 0x%x\n", DATA_START, DATA_END);
    printf("BSS:             0x%x -> 0x%x\n", BSS_START, BSS_END);
    printf("PHYSICAL HEAP:   0x%x -> 0x%x\n", PHYSICAL_HEAP_START, PHYSICAL_HEAP_START + PHYSICAL_HEAP_SIZE);
    printf("-----\n");
    printf("Paging:\n");
    printf("Amount of frames: %i\n", num_frames);

    managed_frames_address_start = allign_address(BSS_END, FRAME_SIZE);
    first_frame_address = allign_address(
        PHYSICAL_HEAP_START + FRAMES_FOR_MANAGING * FRAME_SIZE, FRAME_SIZE);
    printf("Location of frame management: 0x%x\n",
           (long)managed_frames_address_start);
    printf("First frame adress: 0x%x\n", (long)first_frame_address);
    for (uint32_t i = 0; i < num_frames; i++) {
        set_pageframe_free(
            (PageframeFlags *)(managed_frames_address_start + i));
    }
}

void *kalloc_frame() {
    uint32_t currentIndex = 0;
    PageframeFlags *currentFrameFlags =
        (PageframeFlags *)managed_frames_address_start;
    while (!is_free(currentFrameFlags)) {
        currentFrameFlags = currentFrameFlags + 1;
        currentIndex += 1;
    }
    set_pageframe_allocated(currentFrameFlags);
    pageframe_t addr = first_frame_address + currentIndex * FRAME_SIZE;

    printf("Allocated frame %i at address 0x%x\n", currentIndex, (long)addr);
    return (void *)addr;
};
void kfree_frame(void *frame) {
    uint32_t index = get_frame_number((pageframe_t)frame);
    set_pageframe_free((PageframeFlags *)managed_frames_address_start + index);
}




