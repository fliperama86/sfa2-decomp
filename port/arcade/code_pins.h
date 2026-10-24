/* Code ranges and data pins of the game's module chunks (port/arcade/README.md, "Playing without the disc"). The port runs its own C
 * for every function of a module, so the bytes of a module's functions are never executed; the runtime reads them only to check the
 * chunk's identity (modules.c pins each image by the SHA-256 of its chunk). A pack does not need them: its module chunks hold zeros
 * where the functions are, and the runtime checks such a chunk against its data pin instead, the SHA-256 of the chunk with the same
 * ranges zeroed. The ranges are the functions of the image that the matching build establishes (ps1/src/build.toml: exact sizes) and
 * the inventory's other functions of the image (ps1/inventory/modules.tsv) whose last two instructions are `jr ra` and its delay slot;
 * nothing else, and never the chunk's first word (the game's disc stream checks it against the archive's header, check_stream_word,
 * and reads the file again when they differ). The generated table (code_pins_table.c, from code_pins.py --c-table) has only offsets,
 * sizes and hashes. */
#ifndef ARCADE_CODE_PINS_H
#define ARCADE_CODE_PINS_H

#include <stddef.h>
#include <stdint.h>

struct arcade_code_range {
    uint32_t offset, size;   /* in the chunk */
};

struct arcade_code_pin {
    const char *image;                        /* the image's name in the build ("slot04_10") */
    uint32_t size;                            /* bytes of the chunk */
    unsigned char pin[32];                    /* the SHA-256 of the chunk as on the disc (the build's own pin, repeated) */
    unsigned char data_pin[32];               /* the SHA-256 of the chunk with the ranges zeroed */
    const struct arcade_code_range *ranges;   /* sorted, not overlapping, inside the chunk */
    unsigned count;
};

extern const struct arcade_code_pin arcade_code_pins[];
extern const unsigned arcade_code_pin_count;

/* The row of an image, or NULL. */
const struct arcade_code_pin *arcade_code_pin_find(const char *image);

/* Zero p's ranges in bytes (size bytes): 0, or -1 when size is not p->size. */
int arcade_code_zero(const struct arcade_code_pin *p, unsigned char *bytes, size_t size);

#endif
