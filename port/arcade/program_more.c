#include "program_more.h"
#include "program.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The effects' lists are fighter.c's conversion of the arcade's art records (the rules and why are in fighters.py: Converter
 * and model_build), with the cells the PS1 leaves out (the descriptor's drops, handled by arcade_fighter_build), and with
 * the PS1's pictures appended after: their one position stream and the last empty list. program_map.py's effects_model is
 * the specification and tools/test_arcade_program_more.py checks this code against it. */

#define EMPTY_LIST_BYTES 12u

static int fail(char *err, size_t errsize, const char *fmt, ...)
{
    if (err && errsize) {
        va_list ap;
        va_start(ap, fmt);
        vsnprintf(err, errsize, fmt, ap);
        va_end(ap);
    }
    return -1;
}

void arcade_effects_free(struct arcade_effects_out *out)
{
    free(out->data);
    free(out->known);
    memset(out, 0, sizeof *out);
}

int arcade_effects_build(const uint8_t *program, size_t program_size, const struct arcade_effects *e,
                         struct arcade_effects_out *out, char *err, size_t errsize)
{
    struct arcade_fighter f;
    struct arcade_fighter_out fo;
    uint32_t total, lists_cap, pos_end;
    uint8_t *data, *known;

    memset(out, 0, sizeof *out);
    if (err && errsize) err[0] = 0;
    if ((e->segment_count && !e->segments) || (e->drop_count && !e->drops)) return fail(err, errsize, "effects: descriptor incomplete");
    if (e->gap_size > e->lists_size || e->lists_size - e->gap_size < EMPTY_LIST_BYTES) return fail(err, errsize, "effects: the gap does not fit the lists");
    if (e->picture_columns && e->picture_rows > 64) return fail(err, errsize, "effects: %u picture rows", e->picture_rows);
    total = e->positions_size + e->lists_size;
    lists_cap = e->lists_size - e->gap_size;
    /* the records' lists: all of the room but the last empty list, which comes after the pictures' stream */
    memset(&f, 0, sizeof f);
    f.file = "effects";
    f.positions_room = e->positions_size;
    f.lists_room = lists_cap - EMPTY_LIST_BYTES;
    f.segments = e->segments;
    f.segment_count = e->segment_count;
    f.drops = e->drops;
    f.drop_count = e->drop_count;
    if (arcade_fighter_build(program, program_size, &f, &fo, err, errsize)) return -1;
    if (fo.lists_size != f.lists_room) {
        fail(err, errsize, "effects: the lists come to %u bytes, the descriptor leaves %u", (unsigned)fo.lists_size + EMPTY_LIST_BYTES, (unsigned)lists_cap);
        goto bad;
    }
    pos_end = fo.positions_size;
    data = calloc((size_t)total + 1, 1);
    known = calloc((size_t)total + 1, 1);
    if (!data || !known) {
        fail(err, errsize, "out of memory");
        free(data);
        free(known);
        goto bad;
    }
    memcpy(data, fo.positions, pos_end);
    /* the pictures' one stream: a row of columns tiles from the origin, then rows below, each starting at x = 0 */
    if (e->picture_columns && e->picture_rows) {
        unsigned r, len = 0;
        if (6u * e->picture_rows > e->positions_size - pos_end) {
            fail(err, errsize, "the positions need more than their room of %u bytes", (unsigned)e->positions_size);
            goto bad_data;
        }
        for (r = 0; r < e->picture_rows; r++, len += 6) {
            unsigned dx = r ? (unsigned)(-16 * (int)(e->picture_columns - 1u)) & 0xffffu : 0, dy = r ? 16u : 0u, run = e->picture_columns - 1u;
            data[pos_end + len] = (uint8_t)dx;
            data[pos_end + len + 1] = (uint8_t)(dx >> 8);
            data[pos_end + len + 2] = (uint8_t)dy;
            data[pos_end + len + 3] = (uint8_t)(dy >> 8);
            data[pos_end + len + 4] = (uint8_t)run;
            data[pos_end + len + 5] = (uint8_t)(run >> 8);
        }
        pos_end += len;
    }
    if (pos_end > 0xffffu) {
        fail(err, errsize, "a position offset of %u does not fit 16 bits", (unsigned)pos_end);
        goto bad_data;
    }
    memset(known, 1, e->positions_size);
    memcpy(data + e->positions_size, fo.lists, fo.lists_size);
    memset(known + e->positions_size, 1, fo.lists_size);
    /* the gap (unknown), then the last empty list: one cell, tile 0, the position offset the end of the streams */
    data[e->positions_size + fo.lists_size + e->gap_size] = 1;
    data[e->positions_size + fo.lists_size + e->gap_size + 8] = (uint8_t)pos_end;
    data[e->positions_size + fo.lists_size + e->gap_size + 9] = (uint8_t)(pos_end >> 8);
    memset(known + e->positions_size + fo.lists_size + e->gap_size, 1, EMPTY_LIST_BYTES);
    arcade_fighter_free(&fo);
    out->data = data;
    out->known = known;
    out->size = total;
    return 0;
bad_data:
    free(data);
    free(known);
bad:
    arcade_fighter_free(&fo);
    return -1;
}

/* ---- relocations */

int arcade_relocs_apply(const struct arcade_reloc *r, unsigned count, uint8_t *ram, size_t ram_size)
{
    unsigned i;
    int rc = 0;
    for (i = 0; i < count; i++) {
        uint32_t off;
        if (r[i].location < ARCADE_PS1_DATA_START || r[i].location - ARCADE_PS1_DATA_START > ram_size || ram_size - (r[i].location - ARCADE_PS1_DATA_START) < 4) {
            rc = -1;
            continue;
        }
        off = r[i].location - ARCADE_PS1_DATA_START;
        ram[off] = (uint8_t)r[i].target;
        ram[off + 1] = (uint8_t)(r[i].target >> 8);
        ram[off + 2] = (uint8_t)(r[i].target >> 16);
        ram[off + 3] = (uint8_t)(r[i].target >> 24);
    }
    return rc;
}

int arcade_relocs_valid(const struct arcade_reloc *r, unsigned count)
{
    unsigned i;
    for (i = 0; i < count; i++) {
        if ((r[i].location & 3u) || r[i].location < ARCADE_PS1_DATA_START || r[i].location > ARCADE_PS1_DATA_END - 4u) return -1;
        if (i && r[i].location <= r[i - 1].location) return -1;
    }
    return 0;
}
