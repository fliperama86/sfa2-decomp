/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot0fRec7d80 data_800f7d80_slot0f[];
extern u8 data_800f8020_slot0f[][16];
void format_decimal_padded_slot0f(u8 *buf, unsigned int n, int pos, int flags);
void copy_string_with_suffix_by_mode_slot0f(u8 *src, u8 *dst, int mode);

void build_win_count_text_record_slot0f(Object *obj, int a) {
    Slot0fRec7d80 *e;
    FrameRecord *fr;
    u8 buf[16];

    fr = (FrameRecord *)ptr_8019040c;
    fr += (s16)obj->health;
    e = &data_800f7d80_slot0f[obj->table_index];
    if (obj->table_index == 0) {
        e->width = 0x10;
        e->height = 0x10;
        e->mode_param = 2;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        format_decimal_padded_slot0f(buf, fr->field_0c, 7, 0);
        if (fr->field_0c == 1) {
            copy_string_with_suffix_by_mode_slot0f(buf, data_800f8020_slot0f[obj->table_index], 2);
        } else {
            copy_string_with_suffix_by_mode_slot0f(buf, data_800f8020_slot0f[obj->table_index], 3);
        }
        e->data_ptr = data_800f8020_slot0f[obj->table_index];
        e->pos_x = obj->base_x + 0x28;
    } else {
        e->width = 8;
        e->height = 0x10;
        e->mode_param = 1;
        e->field_00 = 0;
        e->field_0b = 0x18;
        if (a == 1) {
            e->field_0b = 0x1d;
        }
        format_decimal_padded_slot0f(buf, fr->field_0c, 7, 0);
        if (fr->field_0c == 1) {
            copy_string_with_suffix_by_mode_slot0f(buf, data_800f8020_slot0f[obj->table_index], 2);
        } else {
            copy_string_with_suffix_by_mode_slot0f(buf, data_800f8020_slot0f[obj->table_index], 3);
        }
        e->data_ptr = data_800f8020_slot0f[obj->table_index];
        e->pos_x = obj->base_x + 0x40;
    }
    e->pos_y = obj->base_y;
    e->pos_x = e->pos_x - 8;
    e->pos_y = e->pos_y - 8;
}
