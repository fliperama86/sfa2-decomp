/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e78e0_slot0f(Object *obj);

void func_800e7748_slot0f(Object *obj) {
    Object *p;
    FrameRecord *fr;
    u8 t = obj->state_1;
    obj->state_1 = t + 1;
    func_800e78e0_slot0f(obj);
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x97;
        p->variant_index = 0;
        p->ot_depth = 1;
        p->pos_x = obj->spawn_x;
        p->pos_y = obj->spawn_y;
        p->table_index = obj->table_index;
        p->health = obj->health;
    }
    p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->active_count = 1;
        p->field_02 = 0x97;
        p->variant_index = 1;
        p->ot_depth = 2;
        p->pos_x = obj->spawn_x;
        p->pos_y = obj->spawn_y;
        p->table_index = obj->table_index;
        p->health = obj->health;
    }
    fr = (FrameRecord *)ptr_8019040c;
    if (fr[obj->table_index].offset_index != 0) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x97;
            p->variant_index = 2;
            p->ot_depth = 0;
            p->pos_x = obj->spawn_x;
            p->pos_y = obj->spawn_y;
            p->table_index = obj->table_index;
            p->health = obj->health;
        }
    }
}
