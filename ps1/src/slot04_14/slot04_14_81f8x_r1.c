/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack them (inferred). */
void func_801b8354_slot04_14(Object *obj);
void func_801b8430_slot04_14(Object *obj, u8 arg);

void func_801b81f8_slot04_14(Object *obj) {
    Object *other;
    u8 palette;

    obj->state_0++;
    other = obj->parent;
    if (game_state.field_64 != 0) {
        obj->field_0f = 0;
        obj->field_0e = 0;
        obj->facing_side = 0;
        obj->ot_depth = 6;
        obj->field_1c = other->field_1c;
        /* The original stores 6 to ot_depth twice, here and before the copy of field_1c. Without this second store this function differs from the original in 3 instruction slots. */
        obj->ot_depth = 6;
        obj->field_90 = other->field_90;
        obj->data_ptr_a = other->data_ptr_a;
        obj->data_ptr_b = other->data_ptr_b;
        obj->spawn_x = other->spawn_x;
        obj->spawn_y = other->spawn_y;
        obj->pos_x = box_margin[0] + 0xb8;
        obj->pos_y = 0x78 - data_801aa5ea[0];
        obj->image_row = 0x1f;
        obj->spawn_flag = 1;
        obj->countdown = 0x15;
        obj->accel_x = 6;
        obj->variant_index_2 = 0;
        palette = obj->table_index;
        if ((s16)other->health != 0x90) {
            palette++;
        }
        obj->table_index = palette;
        func_801b8430_slot04_14(obj, palette);
        if (other->side == 0) {
            func_80130768(obj, 8, data_1f8000b4);
        } else {
            func_80130768(obj, 8, data_1f800164);
        }
        func_8011ffdc(obj);
    } else {
        func_801b8354_slot04_14(obj);
    }
}
