/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Rec data_8002bcb4_slot12[];
extern u8 data_8002bf14_slot12[][16];
void format_hex_padded_slot12(u8 *buf, unsigned int n, int pos, int flags);
void copy_string_with_suffix_by_mode_slot12(u8 *src, u8 *dst, int mode);

void build_hex_value_text_record_slot12(Object *obj, int a) {
    Slot12Rec *e;
    u8 *fr;
    u8 buf[16];

    fr = (u8 *)ptr_8019040c;
    fr += (s16)obj->health << 4;
    e = &data_8002bcb4_slot12[obj->table_index];
    if (game_state.mode_flag_2 == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        format_hex_padded_slot12(buf, *(int *)fr, 7, 0);
        copy_string_with_suffix_by_mode_slot12(buf, data_8002bf14_slot12[obj->table_index], 0);
        e->data_ptr = data_8002bf14_slot12[obj->table_index];
        e->pos_x = obj->base_x + 0x70;
        e->pos_y = obj->base_y;
        e->pos_x = e->pos_x - 8;
        e->pos_y = e->pos_y - 8;
    } else if (obj->table_index == 0) {
        e->field_08 = 0x10;
        e->field_09 = 0x10;
        e->field_0a = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        format_hex_padded_slot12(buf, *(int *)fr, 7, 0);
        copy_string_with_suffix_by_mode_slot12(buf, data_8002bf14_slot12[obj->table_index], 1);
        e->data_ptr = data_8002bf14_slot12[obj->table_index];
        e->pos_x = obj->base_x + 0x38;
        e->pos_y = obj->base_y + 0x20;
        e->pos_x = e->pos_x - 8;
        e->pos_y = e->pos_y - 8;
    } else {
        e->field_08 = 8;
        e->field_09 = 0x10;
        e->field_0a = 1;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        format_hex_padded_slot12(buf, *(int *)fr, 7, 0);
        copy_string_with_suffix_by_mode_slot12(buf, data_8002bf14_slot12[obj->table_index], 0);
        e->data_ptr = data_8002bf14_slot12[obj->table_index];
        e->pos_x = obj->base_x + 0xa0;
        e->pos_y = obj->base_y;
        e->pos_x = e->pos_x - 4;
        e->pos_y = e->pos_y - 8;
    }
}
