/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void set_sequence_from_table_3(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_and_copy_data_to_map(object, *p);
}
