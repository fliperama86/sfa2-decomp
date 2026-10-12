/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80079948_slot00[])(Object *);
void func_800776a8_slot00(Object *obj, short delta);

void func_800773b8_slot00(Object *o) {
    Slot00Obj *obj = (Slot00Obj *)o;
    Object *other;
    if (game_state.field_65 == 0 && game_state.field_a8 == 0) {
        other = o->parent;
        if (obj->countdown != ((other->state_0 << 24) | (other->state_1 << 16) | (other->state_2 << 8))) {
            o->state_0 = o->state_0 + 1;
            return;
        }
        o->pos_x = other->pos_x;
        o->pos_y = other->pos_y;
        o->facing_side = other->facing_side;
        data_80079948_slot00[o->state_1](o);
    }
    func_8011ffdc(o);
}

void func_80077490_slot00(Object *obj) {
    if ((obj->parent->step_flags & 0x80) == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void func_800774c4_slot00(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
    func_800776a8_slot00(obj, 5);
}
