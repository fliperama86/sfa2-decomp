/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_child_object_for_owner_side_slot0b(ModObj *obj);
void set_sequence_from_slot01_table_slot0b(Object *obj, u8 a);

void count_down_then_advance_substate_spawn_child_slot0b(ModObj *obj) {
    int t;
    obj->step_accum += 0x10;
    obj->counter_a += 1;
    obj->counter_b += 1;
    if (ref_game_state.p->phase_state != 1) {
        t = obj->countdown_timer;
        t--;
        obj->countdown_timer = t;
        if ((s16)t >= 0) {
            return;
        }
    }
    obj->step_accum = 0;
    obj->counter_a = 0;
    obj->counter_b = 0;
    obj->substate_index += 1;
    spawn_child_object_for_owner_side_slot0b(obj);
}

void advance_substate_on_phase_2_slot0b(ModObj *obj) {
    if (ref_game_state.p->phase_state == 2) {
        obj->countdown_timer = 0xf;
        obj->substate_index += 1;
        ref_other.p = obj->owner_object;
        set_sequence_from_slot01_table_slot0b((Object *)obj, get_byte_from_table_by_index(ref_other.p->kind));
        if (obj->child != 0) {
            obj->child->mode = 2;
        }
    }
}
