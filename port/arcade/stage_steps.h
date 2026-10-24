/* The animation steps of the stage modules built from the arcade's sequence records (port/arcade/stage_steps.py has the format, the rule and the checks).
 * A step is 12 bytes: a countdown, flags, the module address of a grid (stage_objects.h), a loop offset and a frame index. The PS1's tool made them from the
 * arcade objects' sequence records of 16 bytes (20 with a jump). The generated table (stage_steps_table.c) lists, for each stage file, the runs of steps that
 * the rule builds equal to the disc's (where the first step lies in the module, where its record lies in the arcade program, how many) and the grid each art
 * record became. Only offsets, addresses and counts: no byte of either game. The bytes built are marked known (equal to the disc's). */
#ifndef ARCADE_STAGE_STEPS_H
#define ARCADE_STAGE_STEPS_H

#include <stddef.h>
#include <stdint.h>

struct arcade_stage_step_run {
    uint32_t offset;       /* of the first step, in the module chunk */
    uint32_t record;       /* address of its record in the arcade program */
    uint16_t count;        /* steps, from consecutive records */
};

struct arcade_stage_step_art {
    uint32_t art;          /* address of an art record in the arcade program */
    uint32_t grid;         /* offset in the module chunk of the grid it became */
};

struct arcade_stage_steps {
    const char *file;      /* "STAGE00.PAC" */
    const struct arcade_stage_step_run *runs;
    unsigned run_count;
    const struct arcade_stage_step_art *arts;
    unsigned art_count;
};

extern const struct arcade_stage_steps arcade_stage_steps[];
extern const unsigned arcade_stage_steps_count;

const struct arcade_stage_steps *arcade_stage_steps_find(const char *file);

/* Build the steps of `row` into `module` (module_size bytes: the stage file's module chunk; known[i] = 1 for each byte written) from the arcade program.
 * 0, or -1 with a message in err when a record cannot be read, an art record has no grid, or a jump leads to no record of a run: then nothing is written
 * for that run and the others are still built. */
int arcade_stage_steps_build(const uint8_t *program, size_t program_size, const struct arcade_stage_steps *row,
                             uint8_t *module, uint8_t *known, size_t module_size, char *err, size_t errsize);

#endif
