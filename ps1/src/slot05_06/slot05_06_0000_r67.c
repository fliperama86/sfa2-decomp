/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801dd754_slot05_06;
void func_801cdd64_slot05_06(Object *obj, int a);
void func_801cdd0c_slot05_06(Object *obj);
extern void (*data_801dd5c0_slot05_06[])(Object *);

void func_801cdc58_slot05_06(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801dd754_slot05_06;
    if (obj->table_index == p->kind) {
        a = p->frame->frame_param;
        if (a == 0) {
            obj->variant_index = 0;
        } else {
            obj->pos_x = p->pos_x;
            obj->pos_y = p->pos_y;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != a) {
                func_801cdd64_slot05_06(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        func_801cdd0c_slot05_06(obj);
    }
}

void func_801cdd0c_slot05_06(Object *o) {
    o->state_0++;
}

void func_801cdd20_slot05_06(Object *o) {
    Object *p = data_801dd754_slot05_06;

    if ((s32)o == p->linked_object) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}

void func_801cdd5c_slot05_06(Object *o) {
    o->variant_index = 0;
}

void func_801cdd64_slot05_06(Object *obj, int idx) {
    SequenceStep **table;

    obj->variant_index = idx;
    if (obj->sound_side == 0) {
        table = data_1f8000b4;
    } else {
        table = data_1f800164;
    }
    func_80130768(obj, (u8)idx, table);
}

void func_801cddac_slot05_06(Object *obj) {
    data_801dd5c0_slot05_06[obj->state_0](obj);
}
