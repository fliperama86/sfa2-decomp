/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void spawn_three_child_objects(Object *obj) {
    Object *p[3];
    /* s3 gets no value in this function: the three stores of it below write what the register holds on entry, as the original's instructions do. Nothing is claimed here about what that is. init_object_after_hit_and_spawn_three_effects has the same local. */
    s16 s3;

    if (data_80197f10 >= 3) {
        if (obj->field_49 == 0) {
            get_three_pool_objects_sorted((u32 *)p);
            p[0]->active_count = 1;
            p[0]->field_02 = 0xb;
            p[0]->table_index = 1;
            p[0]->field_0e = obj->field_0e;
            p[0]->variant_index = obj->side;
            p[0]->countdown = 5;
            p[0]->field_20 = 5;
            p[0]->vel_x = s3;
            p[0]->image_row = obj->image_row;
            p[0]->parent = obj;
            p[0]->spawn_mode = 5;
            p[0]->spawn_x = 0x60;
            p[0]->spawn_y = 0x1e0;
            p[1]->active_count = 1;
            p[1]->field_02 = 0xb;
            p[1]->table_index = 2;
            p[1]->field_0e = obj->field_0e;
            p[1]->variant_index = obj->side;
            p[1]->countdown = 10;
            p[1]->field_20 = 10;
            p[1]->vel_x = s3;
            p[1]->image_row = obj->image_row + 3;
            p[1]->parent = obj;
            p[1]->spawn_mode = 5;
            p[1]->spawn_x = 0x60;
            p[1]->spawn_y = 0x1e0;
            p[2]->active_count = 1;
            p[2]->field_02 = 0xb;
            p[2]->table_index = 3;
            p[2]->field_0e = obj->field_0e;
            p[2]->variant_index = obj->side;
            p[2]->countdown = 0xe;
            p[2]->field_20 = 0xe;
            p[2]->vel_x = s3;
            p[2]->image_row = obj->image_row + 4;
            p[2]->parent = obj;
            p[2]->spawn_mode = 5;
            p[2]->spawn_x = 0x60;
            p[2]->spawn_y = 0x1e0;
        }
    }
}
