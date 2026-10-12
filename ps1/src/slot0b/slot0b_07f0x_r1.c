/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 *data_801e46b4_slot0b[];
void set_mode_and_child_mode_slot0b(ModObj *obj);
void build_scaled_rotated_quads_from_layout_slot0b(ModObj *obj, void *a, int b, int c);

void update_countdown_and_counters_slot0b(ModObj *obj) {
    obj->step_accum += 0x10;
    obj->counter_a += 0xffff;
    obj->counter_b += 0xffff;
    obj->countdown_timer = (s16)obj->countdown_timer - 1;
    if ((s16)obj->countdown_timer < 0) {
        obj->mode = 3;
    }
}
