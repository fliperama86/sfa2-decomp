/* Text strings of the screen modules, made from the arcade's text (port/arcade/README.md, "Endings"). The PS1 keeps a string as its
 * characters, then "@@" (the game's end of text), then zero bytes up to the string's size (at least one; the next string starts at a
 * multiple of 4). The arcade keeps the same text in one of two forms:
 *   CODES  a 16-bit big-endian word a character (the 68000's view): 0x0000 is a space; any other word has 0x00 or 0x80 in its high
 *          byte (the arcade's own flag, not kept) and the character's ASCII code with bit 7 set in its low byte (0xa1..0xfe);
 *   PLAIN  a byte a character, its ASCII code (0x20..0x7e).
 * The generated table (texts_table.c, from texts.py --c-table) has, per module chunk, each string's place and size in the chunk and
 * the address and form of its characters in the arcade program: only offsets, addresses and counts, no byte of either game. */
#ifndef ARCADE_TEXTS_H
#define ARCADE_TEXTS_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_TEXT_CODES 0u
#define ARCADE_TEXT_PLAIN 1u

struct arcade_text_string {
    uint32_t offset;    /* in the module chunk */
    uint32_t source;    /* address of the first character in the arcade program (even for CODES) */
    uint16_t chars;     /* characters */
    uint16_t size;      /* PS1 bytes: chars + 2 + the zero bytes */
    uint8_t form;       /* ARCADE_TEXT_CODES or ARCADE_TEXT_PLAIN */
};

struct arcade_text_module {
    const char *file;                          /* "END00.PAC" */
    uint16_t slot;                             /* table 0 slot of the module chunk */
    uint32_t size;                             /* bytes of the chunk */
    const struct arcade_text_string *strings;  /* sorted by offset, not overlapping, inside the chunk */
    unsigned count;
};

extern const struct arcade_text_module arcade_text_modules[];
extern const unsigned arcade_text_module_count;

/* The string's s->size bytes into out: 0, or -1 when its characters lie outside the program, a CODES word or PLAIN byte is not a
 * character of its form, or chars + 3 > size. Nothing is written past out[s->size). */
int arcade_text_build(const uint8_t *program, size_t program_size, const struct arcade_text_string *s, uint8_t *out);

/* The module's own order and bounds (as the comment of `strings` says, sizes as arcade_text_build requires): 0 or -1. */
int arcade_text_module_valid(const struct arcade_text_module *m);

#endif
