/* The talk before a fight in the arcade's English (port/arcade/README.md, "Talk before a fight"). The PS1 (Japanese) draws each box of a
 * talk as two 128 x 48 sprites of a text page (a 256 x 256 picture of 4 bits a pixel, the low nibble first, sent to video memory at
 * (768,0)): box b is rows 48 * b to 48 * b + 47, three lines of 16 rows. The shared module (boss_demo) steps through the file's script,
 * 6-byte records (w0: box, side; w1: timer; w2: the box's line count, which centres the box vertically), one script a file.
 * The arcade (US) keeps the same talks in English in the 16-bit text form of texts.h: a talk is up to 4 boxes of 3 lines of 32 words.
 * This code draws a talk's English into a page with the arcade's 8 x 16 font (quotes.h gives the rule: a character c from 0x21 to
 * 0x7e is the scroll 1 pieces TOP and TOP + 0x10; a space or 0x8000 word is blank): line l of box b at row 48 * b + 16 * l, column k
 * of the line at x = 8 * k, each pixel the arcade's value plus one (the arcade's 15, transparent, is 0); the rest of the page is 0.
 * And it writes the English line count of each box into the w2 of the script's records. Only addresses, offsets and counts are here
 * and in the generated table (talk_table.c, from talk.py --c-table): no byte of either game. */
#ifndef ARCADE_TALK_H
#define ARCADE_TALK_H

#include <stddef.h>
#include <stdint.h>

#define ARCADE_TALK_COLUMNS 32u
#define ARCADE_TALK_LINES 3u          /* lines a box */
#define ARCADE_TALK_BOXES 4u          /* boxes a page */
#define ARCADE_TALK_PAGE 32768u       /* bytes of a page */

struct arcade_talk_page {
    const char *file;          /* "RDM00.PAC" */
    uint32_t text;             /* arcade program address of the talk's first line (box 0, line 0) */
    uint8_t boxes;             /* boxes the arcade's talk has (1 to 4); a page box beyond them repeats the last one, and the script ends at
                                * the first record that shows such a box (see arcade_talk_script_build) */
    uint8_t box_of[ARCADE_TALK_BOXES];   /* for each box of the page, the arcade box drawn there (the script's order) */
    uint32_t script;           /* module offset of the file's script (its first 6-byte record) */
    uint16_t records;          /* records of the script, the last one included (its w0 has bit 15 set) */
};

struct arcade_talk {
    uint16_t page_table, page_slot;    /* the text page chunk (table 2, slot 0, the second chunk of that type: see talk.py) */
    uint16_t page_ordinal;
    uint16_t module_slot;              /* table 0 slot of the module */
    const struct arcade_talk_page *pages;
    unsigned count;
};

extern const struct arcade_talk arcade_talk;

/* The page of p into out (ARCADE_TALK_PAGE bytes). -1 with a message when a character is outside the font or the text lies outside the
 * program. */
int arcade_talk_page_build(const struct arcade_talk_page *p, const uint8_t *program, size_t program_size, const uint8_t *tiles,
                           size_t tiles_size, uint8_t *out, char *err, size_t errsize);

/* The w2 of each record of p's script in module (msize bytes; changed gets a 1 at each byte written): the English line count of the box
 * the record shows (at least 1). The first record that shows a page box b with box_of[b] >= boxes gets bit 15 of its w0 instead, which
 * ends the talk there (advance_pass_or_call_end_by_high_bit reads only that bit of an ending record); the records after it are left.
 * -1 with a message when the script lies outside the module, a record names a box past 3, or the last record has no bit 15. */
int arcade_talk_script_build(const struct arcade_talk_page *p, const uint8_t *program, size_t program_size, uint8_t *module,
                             uint8_t *changed, size_t msize, char *err, size_t errsize);

#endif
