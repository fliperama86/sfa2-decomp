/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_800f7d34_slot0f;
extern u8 data_800f7d4c_slot0f[];
extern Slot12Cell data_800f7e90_slot0f[];
void init_two_prims_with_draw_mode_slot0f(Object *obj, Slot12Prim *p, int w, int h);
void set_base_and_spawn_from_index_table_slot0f(Object *obj);
void set_health_from_table_index_slot0f(Object *obj);

void init_slot_object_by_table_index_slot0f(Object *obj) {
    u8 *p;
    int i;
    obj->pos_y = 0;
    obj->field_01 = 0;
    obj->ot_depth = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->table_index_2 = 0;
    obj->pos_x = 0;
    obj->field_0c = 0;
    obj->pos_x = 0;
    obj->pos_y = 0x100;   /* after the zero stores: this order gives the original's register use */
    obj->state_0++;
    if (obj->table_index == 5) {
        data_800f7d34_slot0f = 0;
        i = 4;
        p = data_800f7d4c_slot0f;
        do {
            *p = 0;
            p--;
            i--;
        } while (i >= 0);
        if (obj->table_index == 5) goto tail;
    }
    init_two_prims_with_draw_mode_slot0f(obj, (Slot12Prim *)((u8 *)data_800f7e90_slot0f + obj->table_index * 64), 0x20, 0x20);
tail:
    set_base_and_spawn_from_index_table_slot0f(obj);
    set_health_from_table_index_slot0f(obj);
}
