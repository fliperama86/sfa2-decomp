/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80023d9c_slot01[];
void load_and_store_16x1_image_slot01(void);

void spawn_child_object_and_load_image_slot01(Object *o) {
    Slot01Obj *obj = (Slot01Obj *)o;
    Object *s = o->parent;
    Object *p = pop_object_from_second_pool();
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
    load_and_store_16x1_image_slot01();
}
