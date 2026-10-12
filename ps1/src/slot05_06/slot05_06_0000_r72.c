/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd6a0_slot05_06[];
extern ObjectFn data_801dd6c0_slot05_06[];
int func_801ce6f8_slot05_06(Object *obj);

void func_801ce330_slot05_06(Object *obj) {
    if (game_state.gate_flag != 0) {
        obj->state_0 = 2;
    } else {
        data_801dd6a0_slot05_06[obj->table_index](obj);
    }
}

void bob_pos_y_and_advance_sequence_slot05_06(Object *obj) {
    int t;
    int a;

    t = obj->variant_index + 1;
    obj->variant_index = t & 0x1f;
    a = 1;
    if (t & 0x10) {
        a = -1;
    }
    obj->pos_y -= a;
    advance_sequence_checked(obj);
    func_8011ffdc(obj);
}

void func_801ce3e8_slot05_06(Object *o) {
    data_801dd6c0_slot05_06[o->state_1](o);
    func_8011ffdc(o);
}

void func_801ce43c_slot05_06(Object *obj) {
    if ((func_801ce6f8_slot05_06(obj) << 16) > 0) {
        obj->vel_x = 0x2000;
        if ((obj->facing_side ^ (obj->table_index & 1)) != 0) {
            obj->vel_x = -0x2000;
        }
        obj->vel_y = 0x1c000;
        obj->accel_y = -0x1800;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        obj->state_1++;
    }
    advance_sequence_checked(obj);
}
