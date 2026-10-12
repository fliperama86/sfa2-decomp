/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002ceb8_slot01[];
void func_80012438_slot01(Object *obj);
void func_800124f4_slot01(Object *obj);
void func_80012714_slot01(Object *obj, u8 *a, int b, int c);

void func_80012344_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->parent;
    if (game_state.status_flags & 0x80) {
        func_80012438_slot01(o);
    } else {
        int t;
        obj->pos_x += o->vel_x;
        o->vel_x += o->accel_x;
        obj->pos_y += o->vel_y;
        o->vel_y += o->accel_y;
        o->table_index_2 += 8;
        if (o->countdown & 1) {
            o->field_20++;
            o->y_scale++;
        }
        t = o->countdown - 1;
        o->countdown = t;
        if ((s16)t < 0) {
            func_80012438_slot01(o);
        }
    }
    func_80012714_slot01(o, data_8002ceb8_slot01, s->side, data_801a27d0);
}

void func_80012438_slot01(Object *obj) {
    obj->table_index_2 = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->state_0++;
    obj->pos_x = obj->health;
    obj->pos_y = obj->y_offset;
    func_800124f4_slot01(obj);
}

void func_8001247c_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *child = obj->child;
    o->state_0 = 3;
    if (child != 0) {
        child->state_0 = 2;
    }
}

void func_8001249c_slot01(Object *obj) {
    func_80012714_slot01(obj, data_8002ceb8_slot01, obj->parent->side, data_801a27d0);
}
