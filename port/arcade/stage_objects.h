/* The object grids of the stage modules built from the arcade's art records (port/arcade/stage_objects.py has the format, the rule and the checks). A grid is
 * columns, rows, a palette row, an origin and a table of tile numbers: the PS1's tool made it from an art record, numbering the tiles of an object (a run of
 * grids) once each. The generated table (stage_objects_table.c) lists, for each stage file, the grids that the rule builds equal to the disc's: where each lies in
 * the module, the address of its art record in the arcade program, and the range of tile numbers its object uses. Only offsets, addresses and numbers:
 * no byte of either game. The bytes built are marked known (equal to the disc's), so the runtime serves them as it does the stage module's other
 * built bytes. */
#ifndef ARCADE_STAGE_OBJECTS_H
#define ARCADE_STAGE_OBJECTS_H

#include <stddef.h>
#include <stdint.h>

#include "stage.h"

struct arcade_stage_grid {
    uint32_t offset;       /* in the module chunk */
    uint32_t art;          /* address of the art record in the arcade program */
    uint16_t lo, hi;       /* the tile numbers of the grid's object, lowest and highest */
};

struct arcade_stage_objects {
    const char *file;      /* "STAGE00.PAC" */
    const struct arcade_stage_grid *grids;
    unsigned grid_count;
};

extern const struct arcade_stage_objects arcade_stage_objects[];
extern const unsigned arcade_stage_objects_count;

const struct arcade_stage_objects *arcade_stage_objects_find(const char *file);

/* Build the grids of `row` into `module` (module_size bytes: the stage file's module chunk; known[i] = 1 for each byte written) from the arcade program and
 * graphics. objects and shared are the stage's tile runs (the object tile chunk's, then the shared effect chunk's: a tile number counts through both). 0, or -1
 * with a message in err when a record cannot be read or a cell names no single tile: then nothing is written for that grid and the others are still built. */
int arcade_stage_objects_build(const uint8_t *program, size_t program_size, const uint8_t *tiles, size_t tiles_size,
                               const struct arcade_stage_run *objects, unsigned object_count,
                               const struct arcade_stage_run *shared, unsigned shared_count,
                               const struct arcade_stage_objects *row, uint8_t *module, uint8_t *known, size_t module_size, char *err, size_t errsize);

#endif
