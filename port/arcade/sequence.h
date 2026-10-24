/* A fighter module's animation steps, frame records and sequence pointer table, built from the arcade program's
 * sequence records. The rules are those of the model in sequences.py (its model_build function is the specification;
 * tools/test_arcade_sequence.py and `sequences.py --compare-c` check this code against it). Only addresses, counts and
 * indices are here, no byte of either game.
 *
 * The PS1 kept only part of the arcade's records and added steps of its own, which are not in the arcade bytes. The
 * generated table (sequences_table.c) says which arcade records make which PS1 steps (the runs), and where the PS1's own
 * steps lie (the own spans: step indices only). This code builds the steps of the runs, the frame records they use and
 * the pointer table entries of the slices, and leaves the rest, marked as not built, to the disc: the runtime serves a
 * built range only where it equals the disc's. */
#ifndef ARCADE_SEQUENCE_H
#define ARCADE_SEQUENCE_H

#include "fighter.h"
#include <stddef.h>
#include <stdint.h>

#define ARCADE_SEQUENCE_STEP 12u     /* bytes of a step: duration, flags, list offset, loop distance, frame number */
#define ARCADE_SEQUENCE_FRAME 16u    /* bytes of a frame record */

/* The per-file rule (the rules field of struct arcade_sequence). */
#define ARCADE_SEQUENCE_JUMP_WITHOUT_BIT15 1u   /* a record followed 4 bytes later by another one is a jump even with bit 15 clear (PL0E) */

/* A group of `count` arcade sequence records, one after another from `address`. The records of a file lie in one or
 * more groups with tables between them; a record's index counts over the groups in order. */
struct arcade_sequence_group {
    uint32_t address;
    uint16_t count;
};

/* Arcade records `record` .. `record + count - 1` (record indices) make the PS1 steps `step` .. */
struct arcade_sequence_run {
    uint16_t record, step, count;
};

/* A span of PS1 steps that no run makes: the PS1's own steps. */
struct arcade_sequence_span {
    uint16_t first, count;
};

/* One of the arcade's sequence tables: `count` 16-bit displacements, each from the table's own address to a record. */
struct arcade_sequence_table {
    uint32_t address;
    uint16_t count;
};

/* PS1 pointer table slots `slot` .. are the arcade's table entries `entry` .. (counted over the tables in order). */
struct arcade_sequence_slice {
    uint16_t entry, slot, count;
};

/* The PS1 gave this step the frame record number `number`, not the one the order of first use gives. */
struct arcade_sequence_frame {
    uint16_t step, number;
};

/* The arcade keeps a copy of some art records (a second group's sequences point at the copy), and the PS1 gave the copy the list of the
 * original: the art record `alias` has the list of the art record `record`. */
struct arcade_sequence_alias {
    uint32_t alias, record;
};

struct arcade_sequence {
    const char *file;                   /* "PL00.PAC": the same files as arcade_fighters */
    uint32_t load;                      /* link address of the module (its pointers are load + module offset) */
    uint32_t steps;                     /* module offset of the first step; the pointer table follows the last step */
    uint32_t frames;                    /* module offset of frame record 0 (all zero) */
    uint16_t step_count, frame_count, table_entries;
    unsigned rules;                     /* ARCADE_SEQUENCE_* flags */
    uint16_t scale_first, scale_count;  /* records in this range have a flags low byte that is a multiple of 0x18 divided by it */
    const struct arcade_sequence_group *groups;
    unsigned group_count;
    const struct arcade_sequence_run *runs;
    unsigned run_count;
    const struct arcade_sequence_span *own;
    unsigned own_count;
    const struct arcade_sequence_table *tables;
    unsigned table_count;
    const struct arcade_sequence_slice *slices;
    unsigned slice_count;
    const struct arcade_sequence_frame *overrides;
    unsigned override_count;
    const struct arcade_sequence_alias *aliases;   /* sorted by alias; NULL, 0 for most files */
    unsigned alias_count;
};

extern const struct arcade_sequence arcade_sequences[];
extern const unsigned arcade_sequence_count;

/* The row for a fighter file name ("PL00.PAC"), or NULL. A file whose module keeps a second block of steps (its own steps, pointer table
 * and frame records, with their own frame numbering) has a second row of the same name: arcade_sequence_find_nth gives the nth (from 0). */
const struct arcade_sequence *arcade_sequence_find(const char *file);
const struct arcade_sequence *arcade_sequence_find_nth(const char *file, unsigned n);

/* The cell list offset of each art record a fighter's lists were made from (offsets from the start of the lists block). */
struct arcade_sequence_list {
    uint32_t record, offset;
};

struct arcade_sequence_lists {
    struct arcade_sequence_list *item;   /* sorted by record, one each; a record drawn twice keeps its last list */
    size_t count;
};

/* Find where each list of `fo` (built by arcade_fighter_build for `f`) starts and map the art records of f's segments to
 * them. -1, with a message in err, when the lists and the segments disagree or a record is malformed. */
int arcade_sequence_lists(const uint8_t *program, size_t program_size, const struct arcade_fighter *f,
                          const struct arcade_fighter_out *fo, struct arcade_sequence_lists *out, char *err, size_t errsize);
void arcade_sequence_lists_free(struct arcade_sequence_lists *lists);

struct arcade_sequence_out {
    uint8_t *steps, *steps_built;          /* step_count steps of 12 bytes; one byte each, 1 where built; unbuilt bytes are zero */
    uint32_t step_count;
    uint8_t *frames, *frames_built;        /* frame_count records of 16 bytes; record 0 (zeros) is built */
    uint32_t frame_count;
    uint8_t *table, *table_built;          /* table_entries 32-bit little-endian pointers to steps, in the module's address space */
    uint32_t table_entries;
};

/* Build the steps, frame records and pointer table of `s` from the arcade program (the 68000's view) and the list offsets
 * of the same file. Fills `out` (allocated; free it with arcade_sequence_free); -1, with a message in err, when the row does
 * not fit the program or a frame number does not fit the frame records. On failure nothing stays allocated and `out` is
 * zeroed. A step whose art record has no list, or whose jump target has no step, is left unbuilt. */
int arcade_sequence_build(const uint8_t *program, size_t program_size, const struct arcade_sequence *s,
                          const struct arcade_sequence_lists *lists, struct arcade_sequence_out *out, char *err, size_t errsize);
void arcade_sequence_free(struct arcade_sequence_out *out);

#endif
