/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea5f4_slot06_03[];
extern u8 data_801ea604_slot06_03[];
extern u16 data_801ea61c_slot06_03[];
extern u8 data_801ea67c_slot06_03[];
extern SequenceStep *data_801eb3e0_slot06_03[];
extern u16 data_801ea680_slot06_03[];
extern ObjectFn data_801ea698_slot06_03[];
extern SeqRec data_801ea2cc_slot06_03[];
extern SeqRec data_801ea310_slot06_03[];
extern SeqRec data_801ea324_slot06_03[];
extern SeqRec data_801ea418_slot06_03[];
extern SeqRec data_801ea53c_slot06_03[];
extern SeqRec data_801ea560_slot06_03[];
extern SeqRec data_801ea294_slot06_03[];
extern SeqRec data_801ea2a8_slot06_03[];
extern ObjectFn data_801ea6a8_slot06_03[];
extern SequenceStep *data_801eb404_slot06_03[];
extern u16 data_801ea6b8_slot06_03[];

void set_random_facing_and_launch_velocity_slot06_03(Object *obj);
void set_random_position_from_table_slot06_03(Object *obj);
void set_random_variant_sequence_slot06_03(Object *obj);
void set_field_from_random_table_entry_slot06_03(Object *obj);
void init_sequence_cursor_from_record_slot06_03(SeqRec *rec, Slot06Cursor *cur);
void advance_record_cursor_slot06_03(Slot06Cursor *cur);
void init_cursor_from_record_header_slot06_03(Slot06Cursor *cur, SeqRec *rec);

void call_handler_by_state_0_slot06_03(Object *obj) {
    data_801ea5f4_slot06_03[obj->state_0](obj);
}

void init_object_spawn_and_call_three_setups_slot06_03(Object *obj) {
    obj->field_0f = 1;
    obj->base_x = 0x2c0;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->state_0 = obj->state_0 + 1;
    set_random_facing_and_launch_velocity_slot06_03(obj);
    set_random_position_from_table_slot06_03(obj);
    set_random_variant_sequence_slot06_03(obj);
}

void func_801e96cc_slot06_03(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        *(u32 *)&obj->pos_x_frac += obj->vel_x;
        *(u32 *)&obj->pos_y_frac += obj->vel_y;
        obj->vel_x += obj->accel_x;
        if (obj->pos_y < 0) {
            set_random_facing_and_launch_velocity_slot06_03(obj);
            set_random_position_from_table_slot06_03(obj);
            set_random_variant_sequence_slot06_03(obj);
            set_field_from_random_table_entry_slot06_03(obj);
        }
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void release_object_record_slot06_03(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_random_facing_and_launch_velocity_slot06_03(Object *obj) {
    u8 r;
    obj->vel_y = -*(s32 *)(data_801ea604_slot06_03 + ((obj->table_index << 3) & 0x3f8));
    r = get_random_byte() & 1;
    if (r != 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    obj->facing_side = r;
}

void set_random_position_from_table_slot06_03(Object *obj) {
    obj->pos_x = data_801ea61c_slot06_03[(u8)(obj->table_index * 16 + (get_random_byte() & 0xf) * 2)];
    obj->pos_y = 0xe0;
}

void set_random_variant_sequence_slot06_03(Object *obj) {
    u8 v = data_801ea67c_slot06_03[get_random_byte() & 3];
    obj->variant_index = v;
    set_sequence_entry(obj, data_801eb3e0_slot06_03[(u8)(obj->table_index * 3 + v)]);
}

void set_field_from_random_table_entry_slot06_03(Object *obj) {
    obj->field_1c = data_801ea680_slot06_03[(u8)((get_random_byte() & 3) | (obj->variant_index << 2))];
}

void call_handler_by_state_0_2_slot06_03(Object *obj) {
    data_801ea698_slot06_03[obj->state_0](obj);
}

void init_object_eight_cursors_slot06_03(Object *obj) {
    obj->field_01 = 0;
    obj->state_0 = obj->state_0 + 1;
    init_sequence_cursor_from_record_slot06_03(data_801ea2cc_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x3c));
    init_sequence_cursor_from_record_slot06_03(data_801ea310_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x10));
    init_sequence_cursor_from_record_slot06_03(data_801ea324_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x18));
    init_sequence_cursor_from_record_slot06_03(data_801ea418_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x28));
    init_sequence_cursor_from_record_slot06_03(data_801ea53c_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x30));
    init_sequence_cursor_from_record_slot06_03(data_801ea560_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x4c));
    init_sequence_cursor_from_record_slot06_03(data_801ea294_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x54));
    init_sequence_cursor_from_record_slot06_03(data_801ea2a8_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x6c));
}

void init_sequence_cursor_from_record_slot06_03(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
}

void advance_object_eight_sequence_cursors_slot06_03(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x3c));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x10));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x18));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x28));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x30));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x4c));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x54));
            advance_record_cursor_slot06_03((Slot06Cursor *)((u8 *)obj + 0x6c));
        }
    }
}

void release_object_record_2_slot06_03(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_record_cursor_slot06_03(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        init_cursor_from_record_header_slot06_03(cur, rec);
    }
}

void init_cursor_from_record_header_slot06_03(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
    store_rect_entries_and_queue_kinds(rec);
}

void call_handler_by_state_0_3_slot06_03(Object *obj) {
    data_801ea6a8_slot06_03[obj->state_0](obj);
}

void func_801e9bc0_slot06_03(Object *obj) {
    obj->field_0f = 1;
    obj->base_x = 0x2c0;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->spawn_mode = 4;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->image_row = 0;
    obj->state_0 = obj->state_0 + 1;
    set_sequence_entry(obj, data_801eb404_slot06_03[obj->table_index]);
}

void set_object_position_from_tick_table_slot06_03(Object *obj) {
    int a = (game_state.tick_count & 1) << 2;
    int i = (u8)(obj->table_index << 3 | a);
    obj->pos_x = data_801ea6b8_slot06_03[i];
    obj->pos_y = 0xf8 - data_801ea6b8_slot06_03[i + 1];
    obj->facing_side = ((u8 *)data_801ea6b8_slot06_03)[i * 2 + 4];
    if (a != 0) {
        advance_sequence_checked(obj);
    }
    func_8011ffdc(obj);
}

void release_object_record_3_slot06_03(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
