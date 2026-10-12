/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80077050_slot00(Object *obj);
void func_800770b8_slot00(Object *obj);

void func_800766a0_slot00(Object *obj) {
    Slot00Obj *o = (Slot00Obj *)obj;
    int speeds[4] = { 0x38000, 0x40000, 0x50000, 0x48000 };
    int a, b;
    obj->ot_depth = 0;
    obj->state_0 = obj->state_0 + 1;
    ref_other.p = obj->parent;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    a = obj->vel_table_index;
    o->b0_state = 5;
    o->b2_state = 0xb;
    b = 3;
    obj->vel_y = 0;
    obj->countdown = 0;
    obj->table_index = 0;
    o->toggle_flag = 0;
    o->step_count = 0;
    if (obj->active_flag_2 != 0) {
        a = 6;
        b = 1;
    }
    obj->variant_index_2 = b;
    if (obj->facing_side != 0) {
        obj->vel_x = speeds[a >> 1];
    } else {
        obj->vel_x = -speeds[a >> 1];
    }
    if (ref_other.p->side == 0) {
        obj->frames = data_1f8000a8;
    } else {
        obj->frames = data_1f800158;
    }
    func_80138070(obj, 2);
    func_80077050_slot00(obj);
    func_800770b8_slot00(obj);
}
