/* The port's runtime: shared declarations.
 *
 * Every function that can fail returns 0 for success, or -1 with one line
 * (no line end) in the caller's buffer saying why. Nothing here knows an
 * address of the game except the two ranges of the PS1's memory below.
 * disc.c uses plain stdio and no system feature, so a test can build it
 * with any C compiler; the system-specific part is memory.c alone. */
#ifndef PORT_H
#define PORT_H

#include <stddef.h>
#include <stdio.h>

#define PORT_RAM_BASE   0x80000000u
#define PORT_RAM_SIZE   0x00200000u
#define PORT_SCRATCH    0x1f800000u
#define PORT_SCRATCH_ALLOC 0x10000u   /* the system's granularity; the first 1 KB is the scratchpad */
#define PORT_ERR        256           /* size of every error buffer */

/* memory.c: map the PS1's RAM (2 MB, executable) and scratchpad at their own
 * addresses. On failure the line names the range and the system's error. */
int  port_map(char *err, size_t errsize);
void port_unmap(void);

/* disc.c */
struct port_disc {
    FILE *f;
    char path[1024];           /* the image file actually read (the .bin) */
    unsigned long long length; /* its size in bytes */
};

#define PORT_SECTOR 2352u
#define PORT_DATA   2048u
#define PORT_DATA_OFFSET 24u

/* Open a .cue (first FILE "..." BINARY line, path relative to the cue's
 * folder) or a .bin itself. */
int  port_disc_open(struct port_disc *d, const char *path, char *err, size_t errsize);
void port_disc_close(struct port_disc *d);
/* Read the data bytes of the sector range starting at `sector`, `size` bytes. */
int  port_disc_read(struct port_disc *d, unsigned sector, unsigned size, unsigned char *out, char *err, size_t errsize);
/* Find a file by name in the root directory or one folder below it (version
 * suffix and case ignored). */
int  port_disc_find(struct port_disc *d, const char *name, unsigned *sector, unsigned *size, char *err, size_t errsize);
/* From the text of SYSTEM.CNF: the file name of its BOOT line. -1 if none. */
int  port_boot_name(const char *text, size_t length, char *out, size_t outsize);

struct port_program {
    char name[64];
    unsigned sector;    /* of the file on the disc */
    unsigned t_addr, t_size, pc0;
    unsigned b_addr, b_size;
};
/* Parse and load a PS-X EXE image of `size` bytes. `ram` points at the memory
 * that stands for PORT_RAM_BASE (the runtime passes the real address). */
int  port_exe_load(const unsigned char *file, size_t size, unsigned char *ram, struct port_program *p, char *err, size_t errsize);
/* SYSTEM.CNF, the boot file, and port_exe_load of it. */
int  port_program_load(struct port_disc *d, unsigned char *ram, struct port_program *p, char *err, size_t errsize);
/* The target of the last jal before the first break (the entry code's halt
 * after main returns) within the first 64 instructions at pc0 of the loaded
 * memory. -1 if there is no break in the window or no jal before it. */
int  port_entry_scan(const unsigned char *ram, unsigned pc0, unsigned *target);

/* jumps.c */
/* Write the 5-byte jumps and calls for the resident functions. Counts go to
 * the two outputs. `ram` as above. */
int  port_jumps_write(unsigned char *ram, unsigned *with_c, unsigned *without_c, char *err, size_t errsize);
/* The entry of the 5-byte call written for functions without C. */
void port_stop_entry(void);
/* Print the line, flush, end the program. Never returns. */
void port_stop_main_returned(void);

#endif
