/* Full animation sets: a fighter module whose animation the PS1 cut, given the fuller set of another module of the same
 * fighter (port/arcade/README.md, "Full animation sets"). Only addresses, counts and indices are here, no byte of either
 * game.
 *
 * The PS1 loads a fighter's mirror module (both players the same fighter) with more of the arcade's animation than the
 * module of the same fighter against anyone else, whose memory it shares with the other fighter: Zangief's PL16 holds
 * 993 steps and 376 cell lists, his PL10 and PL10X 890 and 272. Both modules run the same code, and their frame records
 * and box tables are the same. The module keeps its code, its frame records, its box tables and its step table where its
 * code reads them; this code writes into it the source's position runs (at the same place: the code reads them at a
 * fixed address), the source's steps (in the room the moved cell lists leave), the step table in the module's own
 * numbering (each entry the source's entry of the same sequence) and the list offsets of the steps that lie outside the
 * step block. The source's cell lists go elsewhere (the port's extra memory, lists_address), which the steps reach: a
 * step's list offset is 32 bits from the module's lists base. The sprite tiles of the source are the runtime's to place
 * (its sprite chunk is larger than the room the PS1 gives the module's). */
#ifndef ARCADE_FULL_SETS_H
#define ARCADE_FULL_SETS_H

#include <stddef.h>
#include <stdint.h>

/* Target step table entries first .. first + count - 1 are the source's entries source_first .. (the same sequences). */
struct arcade_full_run {
    uint16_t first, count, source_first;
};

/* A step outside the step block (in the module's other data) whose list offset follows the lists: module offsets of
 * the step in the target and of the same step in the source. */
struct arcade_full_step {
    uint32_t target, source;
};

struct arcade_full_set {
    const char *file;                   /* the target, "PL10.PAC" */
    const char *source;                 /* the module with the fuller set, "PL16.PAC" */
    uint16_t slot, source_slot;         /* table 0 slots of the two module chunks */
    uint16_t tiles_slot, source_tiles_slot;   /* table 0 slots of the two sprite chunks */
    uint16_t kind;                      /* the player kind that loads the target (its file is entry 2 * kind or 2 * kind + 1) */
    uint32_t load, source_load;         /* link addresses of the modules (their pointers are load + module offset) */
    /* the source's blocks: module offsets and sizes; its step table follows its last step */
    uint32_t source_positions, positions_size, source_lists, lists_size, source_steps;
    uint16_t step_count, source_table_entries;
    /* the target's: where its code reads the position runs and adds the list offsets to (module offsets), where the
     * source's steps go, and the step table that stays in place */
    uint32_t positions, lists, steps, table;
    uint16_t table_entries;
    const struct arcade_full_run *runs;
    unsigned run_count;
    const struct arcade_full_step *outside;
    unsigned outside_count;
};

extern const struct arcade_full_set arcade_full_sets[];
extern const unsigned arcade_full_set_count;

/* The target module chunk (size bytes) with the source's set written in for cell lists at lists_address (the source's
 * lists block copied there as it is): `out` gets the chunk (size bytes) and `changed` (size bytes) a 1 at each byte
 * written. Checks that the blocks lie inside both chunks, that the positions end before the steps and the steps
 * before the table, that every run lies inside both tables and that each source table entry it uses points to a step
 * of the source's steps; -1 with a message in err when not (nothing is valid in out then). */
int arcade_full_build(const struct arcade_full_set *s, const uint8_t *module, uint32_t size, const uint8_t *source,
                      uint32_t source_size, uint32_t lists_address, uint8_t *out, uint8_t *changed, char *err, size_t errsize);

/* The sprite chunk's offset table for the source's tiles placed elsewhere: the game finds tile i at base + entry i (base is
 * where its code reads the chunk, the module's sprite base; decode_compressed_block), so entry i of the table written to out
 * is address + (the source's entry i) - base, where address is where the source's whole chunk (source_size bytes) lies.
 * The source has (its entry 0) / 4 tiles. Writes 4 * *count bytes (room bytes there). -1 with a message in err when the
 * source's table is malformed (entry 0 not a multiple of 4, an entry outside the chunk) or does not fit in room. The rest
 * of the target's chunk is not read: its first word, which the archive's header repeats, is entry 0 of the new table. */
int arcade_full_tiles(const uint8_t *source, uint32_t source_size, uint32_t address, uint32_t base, uint8_t *out, uint32_t room,
                      uint32_t *count, char *err, size_t errsize);

#endif
