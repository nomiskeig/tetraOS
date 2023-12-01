#include "klib.h"
#include <cstddef>
#include <stdint.h>

extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;
extern uint32_t KERNEL_STACK_START;
extern uint32_t KERNEL_STACK_END;

const size_t PAGE_SIZE = 4096;
static size_t num_pages = 0;
static size_t pages_alloc_start;
// size: 1 byte
struct Page {
    uint8_t used : 1;
    uint8_t last : 1;
};
size_t align_address(size_t address) { return (address + 4095) & ~(4095); }

void clear_page(Page *page) {
    page->used = 0;
    page->last = 0;
}

/**
 *
 * This manages pages. Each page has a Page structure in the beginning of the heap, the real, usable area of the page starts after those structures
 * The adress of the first page is pages_alloc_start
 * The first Page structure is located at HEAP_START
 *
 */
void page_init() {
    kprintf("Initializing paging\n");
    kprintf("TEXT: %x -> %x\n", TEXT_START, TEXT_END);
    kprintf("RODATA: %x -> %x\n", RODATA_START, RODATA_END);
    kprintf("DATA: %x -> %x\n", DATA_START, DATA_END);
    kprintf("BSS: %x -> %x\n", BSS_START, BSS_END);
    kprintf("HEAP: %x -> %x\n", HEAP_START, HEAP_SIZE);
    kprintf("KERNEL_STACK: %x -> %x\n", KERNEL_STACK_START, KERNEL_STACK_END);
    kprintf("Page size: %d\n", sizeof(Page));
    num_pages = HEAP_SIZE / PAGE_SIZE;
    kprintf("Number of pages: %d\n", num_pages);
    pages_alloc_start = align_address(HEAP_START + num_pages * sizeof(Page));
    kprintf("Pages allocation start: %x\n", pages_alloc_start);
    // clear pages
    for (size_t i = 0; i < num_pages; i++) {
        Page *page = (Page *)(HEAP_START + i * sizeof(Page));
        clear_page(page);
    }
}

// allocates amount of pages
void *alloc(size_t amount) {
    bool found = true;
    for (size_t i = 0; i < num_pages; i++) {
        Page *page = (Page *)(HEAP_START + i * sizeof(Page));
        if (page->used) {
            continue;
        }
        // found an empty page, check the next amount-1 if empty
        for (size_t j = i + 1; j < i + amount; j++) {
            page = (Page *)(HEAP_START + j * sizeof(Page));
            if (page->used) {
                found = false;
                break;
            }
        }
        if (found) {
            // mark as used and the last one as the last one
            for (int k = i; k < i + amount; k++) {
                page = (Page *)(HEAP_START + k * sizeof(Page));
                page->used = 1;
                if (k == i + amount - 1) {
                    page->last = 1;
                }
            }

            return (uint8_t *)(pages_alloc_start + PAGE_SIZE * i);
        }
    }
    return NULL;
}

void dealloc(void *address) {
    size_t index = ((size_t)address - pages_alloc_start) / PAGE_SIZE;
    Page *page = (Page *)(HEAP_START + index);
    while (!page->last) {
        clear_page(page);
        page += sizeof(Page);
    }
    clear_page(page);
}

void print_allocations() {
    kprintf("Current page allocations:\n");
    bool started = false;
    size_t start_address = 0;
    for (size_t i = 0; i < num_pages; i++) {
        Page *page = (Page *)(HEAP_START + i);
        if (!started && page->used) {
            start_address = pages_alloc_start + PAGE_SIZE * i;
            started = true;
        }
        if (page->last) {
            size_t end_address = pages_alloc_start + PAGE_SIZE * i;
            kprintf("Allocation: 0x%x -> 0x%x\n", start_address, end_address);
            started = false;
        }
    }
    kprintf("End page of allocations\n");
}
