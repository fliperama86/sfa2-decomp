/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801eb960_slot06_01[];

void set_sequence_from_table_by_index_slot06_01(Object *obj, u8 index);

void set_sequence_from_table_by_index_slot06_01(Object *obj, u8 index) {
    obj->accel_x = index;
    set_sequence_entry(obj, data_801eb960_slot06_01[index]);
}
