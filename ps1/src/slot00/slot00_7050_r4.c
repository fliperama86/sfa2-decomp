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
        other = o->field_3c;
        if (obj->field_a4 != ((other->field_04 << 24) | (other->field_05 << 16) | (other->field_06 << 8))) {
            o->field_04 = o->field_04 + 1;
            return;
        }
        o->pos_x = other->pos_x;
        o->pos_y = other->pos_y;
        o->field_0b = other->field_0b;
        data_80079948_slot00[o->field_05](o);
    }
    func_8011ffdc(o);
}

void func_80077490_slot00(Object *obj) {
    if ((obj->field_3c->field_3a & 0x80) == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
}

void func_800774c4_slot00(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
    func_800776a8_slot00(obj, 5);
}
