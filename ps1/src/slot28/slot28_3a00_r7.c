/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_8002be28_slot28[];
extern Object *data_800518bc_slot28[];

void set_sequence_of_fourth_hud_object_2_slot28(Object *obj, int arg) {
    set_sequence_from_table_5(data_800518bc_slot28[3], arg, data_8002be28_slot28);
}
