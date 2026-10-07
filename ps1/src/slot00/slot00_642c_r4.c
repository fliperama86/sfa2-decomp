/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_800797c0_slot00[];
extern ObjectFn data_800797d4_slot00[];

void func_80076810_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    int t;
    ref_other.p = o->field_3c;
    obj->field_b0--;
    if (obj->field_b0 & 0x80) {
        obj->field_b0 = 5;
        o->field_0c = ref_other.p->field_0c;
        t = ref_other.p->field_0d;
        obj->field_b1 ^= 1;
        o->field_0d = t;
        if (obj->field_b1 != 0) {
            o->field_0c = 0xff;
            o->field_0d++;
        }
    }
    data_800797c0_slot00[o->field_05](o);
}

void func_800768d4_slot00(Object *obj) {
    if (game_state.field_65 == 0 && game_state.field_a8 == 0) {
        data_800797d4_slot00[obj->field_06](obj);
    }
    func_8011ffdc(obj);
}
