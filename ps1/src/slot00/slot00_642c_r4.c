/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797c0_slot00[];
extern ObjectFn data_800797d4_slot00[];

void func_80076810_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    int t;
    ref_other.p = o->parent;
    obj->b0_state--;
    if (obj->b0_state & 0x80) {
        obj->b0_state = 5;
        o->field_0c = ref_other.p->field_0c;
        t = ref_other.p->image_row;
        obj->toggle_flag ^= 1;
        o->image_row = t;
        if (obj->toggle_flag != 0) {
            o->field_0c = 0xff;
            o->image_row++;
        }
    }
    data_800797c0_slot00[o->state_1](o);
}

void func_800768d4_slot00(Object *obj) {
    if (game_state.field_65 == 0 && game_state.field_a8 == 0) {
        data_800797d4_slot00[obj->state_2](obj);
    }
    func_8011ffdc(obj);
}
