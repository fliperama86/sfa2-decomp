/* Load the guest image (guest.elf, 32-bit AArch64, see ../tools/guestlink.py) into the machine and read its symbols. */
#ifndef PORT_ELF_H
#define PORT_ELF_H

#include <stddef.h>
#include <stdint.h>

struct elf_symbol {
    const char *name;
    uint32_t address;
    uint32_t size;
};

struct elf_image {
    uint32_t entry;
    struct elf_symbol *symbols;      /* sorted by address, then name */
    size_t count;
    char *strings;
};

/* Copy every loaded segment to the guest's memory and give it its permissions (vm_protect). A segment outside the image's area
 * [VM_IMAGE_BASE, VM_IMAGE_LIMIT), one not on a page boundary and any other fault of the file is -1 with a line. */
int  elf_load(const char *path, struct elf_image *image, char *err, size_t errsize);
void elf_free(struct elf_image *image);
/* The address of a defined symbol by name, 0 on success and -1 when there is none. */
int  elf_find(const struct elf_image *image, const char *name, uint32_t *address);
/* The symbol that `address` lies in or after (within 1 MB of the symbol's start): its name, and in *offset the distance. NULL if none. */
const char *elf_symbol_at(const struct elf_image *image, uint32_t address, uint32_t *offset);

#endif
