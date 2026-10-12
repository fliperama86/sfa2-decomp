/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_8002bc78_slot12;
extern u8 data_8002bc90_slot12[];
extern Slot12Cell data_8002bdd4_slot12[];
void init_two_prims_with_draw_mode_slot12(Object *obj, Slot12Prim *p, int w, int h);
void set_base_and_spawn_from_table_slot12(Object *obj);
void set_health_from_y_offset_and_table_index_slot12(Object *obj);

void init_slot_object_by_table_index_slot12(Object *obj) {
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
        data_8002bc78_slot12 = 0;
        i = 4;
        p = data_8002bc90_slot12;
        do {
            *p = 0;
            p--;
            i--;
        } while (i >= 0);
        if (obj->table_index == 5) goto tail;
    }
    init_two_prims_with_draw_mode_slot12(obj, (Slot12Prim *)((u8 *)data_8002bdd4_slot12 + obj->table_index * 64), 0x20, 0x20);
tail:
    set_base_and_spawn_from_table_slot12(obj);
    set_health_from_y_offset_and_table_index_slot12(obj);
}
