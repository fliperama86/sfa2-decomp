/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void build_block_primitives(Block172 *block) {
    void *p = block->data_ptr;
    build_cell_prim_list_from_history(p, block->block_mode, block->field_20, block);
    fill_cell_record_rects_from_history(p, block->block_mode, block->field_20, block);
}
