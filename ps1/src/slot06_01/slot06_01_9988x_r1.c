/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_from_table_by_index_slot06_01(Object *obj, u8 index);

void init_object_spawn_with_player_parent_slot06_01(Object *obj) {
    int index = 6;
    u8 t;

    obj->field_0f = 1;
    obj->countdown = 0x3f;
    obj->base_x = 0x300;
    obj->base_y = 0x100;
    obj->spawn_x = 0x60;
    obj->spawn_y = 0x1e0;
    t = obj->state_0;
    obj->accel_y = 0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->state_0 = t + 1;
    *(u16 *)&obj->pos_y = 0xf8 - (u16)obj->pos_y;
    if (obj->table_index != 0) {
        Object *p = &player_left;

        if (p->mode_flag_2 != 0 || p->kind != 1) {
            p++;
            if (p->mode_flag_2 != 0 || p->kind != 1) {
                set_sequence_from_table_by_index_slot06_01(obj, 0);
                return;
            }
        }
        index = 0x11;
        obj->parent = p;
        obj->accel_y = 1;
    }
    set_sequence_from_table_by_index_slot06_01(obj, index);
}
