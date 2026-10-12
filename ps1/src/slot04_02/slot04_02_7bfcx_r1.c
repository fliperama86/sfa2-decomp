/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_state_0_to_2_slot04_02(Object *obj);
void func_801b7e34_slot04_02(Object *obj, u8 arg);

/*
 * What it does (inferred, not an original name): a state of a helper object
 * that follows another one (parent). While game_state.field_64 is not 0 it
 * copies fields of the other object, places itself at fixed coordinates, steps
 * table_index up by one unless the other object's health is 0x90, loads a
 * palette copy for that value, starts a sequence table chosen by the other
 * object's side and registers itself. Otherwise it only calls
 * set_state_0_to_2_slot04_02.
 */
void func_801b7bfc_slot04_02(Object *obj) {
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
        /* The original stores 6 to ot_depth twice; without the second store the build differs in 3 instruction lines. */
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
        /* Without the (s16) cast the build differs in 1 instruction line. */
        if ((s16)other->health != 0x90) {
            palette++;
        }
        obj->table_index = palette;
        func_801b7e34_slot04_02(obj, palette);
        if (other->side == 0) {
            set_sequence_from_table_5(obj, 8, data_1f8000b4);
        } else {
            set_sequence_from_table_5(obj, 8, data_1f800164);
        }
        clear_or_set_flag_by_pos_x_limit(obj);
    } else {
        set_state_0_to_2_slot04_02(obj);
    }
}
