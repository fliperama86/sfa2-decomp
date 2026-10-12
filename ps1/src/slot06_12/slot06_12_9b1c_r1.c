/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801904cc;
extern Slot06_12Recf310 data_801ef310_slot06_12[];
extern u8 data_801ef320_slot06_12[];
extern SequenceStep *data_801eee74_slot06_12[];
extern ObjectFn data_801ef330_slot06_12[];
extern u8 data_801eef00_slot06_12[];
extern SeqRec *data_801ef2f4_slot06_12[];

void set_object_spawn_position_and_sequence_slot06_12(Object *obj);
void set_table_index_from_accel_x_high_bits_slot06_12(Object *obj);
void count_down_vel_y_then_next_record_slot06_12(Object *obj);
void advance_accel_y_index_and_load_record_slot06_12(Object *obj);
void set_velocity_from_record_header_slot06_12(Object *obj, SeqRec *rec);

void set_object_spawn_position_and_sequence_slot06_12(Object *obj) {
    s16 r;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    s16 *c = &data_801904cc;
    int i = (u16)*c & 3;

    obj->field_1c = *(u16 *)((u8 *)data_801ef310_slot06_12 + (i << 2));
    obj->pos_y = 0xf8 - *(u16 *)((u8 *)data_801ef310_slot06_12 + (i << 2) + 2);
    r = get_random_byte() & 0xff;
    obj->pos_x = 0xc0;
    if (*c != 3) {
        obj->pos_x = r + 0x40;
    }
    set_sequence_from_table_5(obj, data_801ef320_slot06_12[(*c << 2) + (r & 3)], data_801eee74_slot06_12);
}

void set_table_index_from_accel_x_high_bits_slot06_12(Object *obj) {
    s16 *c = &data_801904cc;

    *c = 0;
    *c = (obj->accel_x >> 8) & 0x7f;
    obj->table_index = *(u8 *)c;
}

void call_handler_by_table_index_slot06_12(Object *obj) {
    if (game_state.field_65 == 0) {
        count_down_vel_y_then_next_record_slot06_12(obj);
        data_801ef330_slot06_12[obj->table_index](obj);
    } else {
        obj->field_01 = 0;
    }
}

void update_table_index_and_respawn_if_nonzero_slot06_12(Object *obj) {
    set_table_index_from_accel_x_high_bits_slot06_12(obj);
    if (data_801904cc != 0) {
        set_object_spawn_position_and_sequence_slot06_12(obj);
        set_flag_and_push_if_mode_8(obj);
    }
}

void update_object_with_step_x_shift_slot06_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_table_index_from_accel_x_high_bits_slot06_12(obj);
        set_object_spawn_position_and_sequence_slot06_12(obj);
    }
    advance_sequence_checked(obj);
    if ((s16)obj->step_flags != 0) {
        obj->pos_x += 8;
    }
    set_flag_and_push_if_mode_8(obj);
}

void respawn_on_negative_step_flags_then_advance_slot06_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_table_index_from_accel_x_high_bits_slot06_12(obj);
        set_object_spawn_position_and_sequence_slot06_12(obj);
    }
    advance_sequence_checked(obj);
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_4_slot06_12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void count_down_vel_y_then_next_record_slot06_12(Object *obj) {
    SeqRec *rec;

    obj->vel_y = obj->vel_y - 1;
    if (obj->vel_y == 0) {
        rec = (SeqRec *)((ObjectWords *)obj)->word_4c;
        rec++;
        if (obj->accel_x < 0) {
            advance_accel_y_index_and_load_record_slot06_12(obj);
        } else {
            set_velocity_from_record_header_slot06_12(obj, rec);
        }
    }
}

void advance_accel_y_index_and_load_record_slot06_12(Object *obj) {
    obj->accel_y = (obj->accel_y + 1) & 0x3f;
    set_velocity_from_record_header_slot06_12(obj, data_801ef2f4_slot06_12[data_801eef00_slot06_12[obj->accel_y]]);
}

void set_velocity_from_record_header_slot06_12(Object *obj, SeqRec *rec) {
    int w;

    ((ObjectWords *)obj)->word_4c = (s32)rec;
    w = rec->header;
    obj->vel_y = w >> 16;
    obj->accel_x = (s16)w;
    store_rect_entries_and_queue_kinds(rec);
}
