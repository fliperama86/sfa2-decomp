/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801e9e6c_slot06_04[];
extern s32 data_801e9e7c_slot06_04[];
extern u8 data_801e9eac_slot06_04[];
extern SequenceStep *data_801eb300_slot06_04[];
extern u16 data_80190542;

void set_flag_and_push_if_mode_8(Object *o);
void update_object_wrapped_x_motion_slot06_04(Object *obj);
void set_vel_x_from_table_index_slot06_04(Object *obj);
void set_pos_x_wrapped_to_scroll_slot06_04(Object *obj, int x);
void set_sequence_via_double_table_lookup_slot06_04(Object *obj);

void call_handler_by_state_0_slot06_04(Object *obj) {
    data_801e9e6c_slot06_04[obj->state_0](obj);
}

void init_object_spawn_from_position_slot06_04(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->base_x = 0x2c0;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->spawn_mode = 4;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->state_0++;
    obj->accel_x = *(u32 *)&obj->pos_x_frac;
    obj->accel_y = 0x1000000 - *(u32 *)&obj->pos_y_frac;
    obj->pos_y += 0x58;
    obj->accel_x += -0x800000;
    set_vel_x_from_table_index_slot06_04(obj);
    set_sequence_via_double_table_lookup_slot06_04(obj);
    update_object_wrapped_x_motion_slot06_04(obj);
}

void update_object_wrapped_x_motion_slot06_04(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        u32 m = 0x3ffffff;
        u32 a = obj->accel_x + obj->vel_x;

        a &= m;
        obj->accel_x = a;
        set_pos_x_wrapped_to_scroll_slot06_04(obj, (a + 0x800000) >> 16);
        advance_sequence_checked(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_slot06_04(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_vel_x_from_table_index_slot06_04(Object *obj) {
    obj->vel_x = data_801e9e7c_slot06_04[obj->table_index];
}

void set_pos_x_wrapped_to_scroll_slot06_04(Object *obj, int arg) {
    s16 x = arg;

    if ((u32)(x - (s16)data_80190542 - (s16)((Slot06Layer *)data_801aa5d4)->scroll_x + 0x40) >= 0x200) {
        x = ((x ^ 0x200) - 0x80) & 0x3ff;
        x += 0x80;
    }
    obj->pos_x = x;
}

void set_sequence_via_double_table_lookup_slot06_04(Object *obj) {
    set_sequence_entry(obj, data_801eb300_slot06_04[data_801e9eac_slot06_04[obj->table_index]]);
}
