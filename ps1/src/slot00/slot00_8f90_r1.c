/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80078f90_slot00(Object *obj) {
    Object *t;
    obj->field_01 = 1;
    obj->field_04++;
    t = obj->field_3c;
    obj->pos_x = (u16)t->pos_x;
    obj->pos_y = (u16)t->pos_y;
    obj->frames = t->frames;
    if (obj->field_66 == 0) {
        obj->sequence = seqs_60_left[obj->field_48];
    } else {
        obj->sequence = seqs_110_right[obj->field_48];
    }
    obj->field_38 = obj->sequence->duration;
    obj->field_3a = obj->sequence->flags;
    obj->field_80 = 1;
    obj->frame = obj->frames + obj->sequence->frame_index;
}

void func_8007904c_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    if (game_state.field_6a == 0) {
        obj->field_10 += o->field_4c;
        o->field_4c += o->field_54;
        if (o->field_4c == 0) {
            o->field_4c = 0;
            o->field_54 = 0;
        }
        o->field_01 = 1;
        if (((game_state.field_1d + o->field_03) & 1) == 0) {
            o->field_01 = 0;
        }
        func_80131094(o);
    } else {
        o->field_04 = 2;
    }
}
