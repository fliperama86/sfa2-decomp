/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80023d9c_slot01[];
void func_8001257c_slot01(void);

void func_800124f4_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->parent;
    Object *p = func_8011f32c();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x23;
        p->table_index = 0;
        p->variant_index = s->side;
        p->sound_side = 0;
        ((ObjectWords *)p)->word_58 = (s32)data_80023d9c_slot01;
        p->vel_x = 0;
        p->vel_y = 0;
        p->facing_side = 0;
        p->parent = o;
        obj->child = p;
    }
    func_8001257c_slot01();
}
