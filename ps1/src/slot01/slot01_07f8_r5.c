/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_80023d00_slot01[];

void set_sequence_from_slot01_table_2_slot01(Object *obj, u8 a) {
    set_sequence_from_table_5(obj, a, data_80023d00_slot01);
}
