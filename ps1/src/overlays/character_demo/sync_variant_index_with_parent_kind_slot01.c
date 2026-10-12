/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80033ba8_slot01;
extern ObjectRef data_80033bac_slot01;
void set_parent_kind_and_load_kind_pointers_slot01(Object *obj, int arg);

void sync_variant_index_with_parent_kind_slot01(Object *obj) {
    data_80033ba8_slot01.p = obj->parent;
    if (obj->table_index != obj->variant_index) {
        obj->table_index = obj->variant_index;
    }
    if (data_80033ba8_slot01.p->kind != obj->variant_index) {
        obj->variant_index = data_80033ba8_slot01.p->kind;
        set_parent_kind_and_load_kind_pointers_slot01(obj, 0);
    } else if ((game_state.mode >> data_80033ba8_slot01.p->side) & 1) {
        obj->state_1++;
        obj->active_state = data_80033ba8_slot01.p->field_d4;
        set_parent_kind_and_load_kind_pointers_slot01(obj, 1);
    }
}

void update_active_state_and_countdown_slot01(Object *obj) {
    Object *p = obj->parent;
    data_80033bac_slot01.p = p;
    if (p->field_d4 != obj->active_state) {
        obj->active_state = p->field_d4;
    }
    if ((obj->step_flags << 16) < 0) {
        obj->health = 0;
    }
    if (*(s16 *)&obj->health == 0) {
        s16 t = *(s16 *)&obj->countdown;
        if (t != 0) {
            obj->countdown = t - 1;
        }
    }
}
