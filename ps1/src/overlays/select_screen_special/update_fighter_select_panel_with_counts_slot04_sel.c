/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern TextBuf *data_801b7f64_slot04_sel[];
extern Slot04SelRec7ff4 *data_801b7ff4_slot04_sel[];
extern Slot04SelRec7fa4 *data_801b7fa4_slot04_sel[];
extern Slot04SelRec84a4 *data_801b84a4_slot04_sel[];
extern Slot04SelRec7ff4 *data_801b82cc_slot04_sel[];
extern TextItem *data_801b8978_slot04_sel[];
extern u8 data_801b9d30_slot04_sel;
extern u8 data_801b9d34_slot04_sel;
extern u8 data_801b7cc4_slot04_sel[];
extern Slot04SelRec7c70 data_801b7c70_slot04_sel[];
extern u8 data_801b7d30_slot04_sel[];

void write_two_digit_decimal_ascii_slot04_sel(u8 *dst, u8 n);
void update_kind_2_4_hold_counters_b_slot04_sel(Object *obj, Slot04SelRec9d38 *rec);
void update_button_sequence_matcher_2_slot04_sel(Object *obj, Slot04SelRec9d38 *rec);
void set_selection_from_direction_buttons_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, int a);
void check_opposite_player_same_selection_2_slot04_sel(void);

void update_fighter_select_panel_with_counts_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    TextBuf *s2 = data_801b7f64_slot04_sel[obj->side];
    Slot04SelRec7ff4 *s3 = data_801b7ff4_slot04_sel[obj->side];
    Slot04SelRec7fa4 *s4 = data_801b7fa4_slot04_sel[obj->side];
    Slot04SelRec84a4 *s5 = data_801b84a4_slot04_sel[obj->side];
    u8 *p;
    u8 n;
    Slot04SelRec7ff4 *a1;
    TextItem *b1;
    int i;
    u8 t;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];

    p = s2->buf + 6;
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + 0x11];
        n += ((u8 *)table_801aa4e8)[obj->side * 42 + 0x13];
    } else {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + obj->kind];
    }
    write_two_digit_decimal_ascii_slot04_sel(p, n);
    p = s2->buf + 0xf;
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + 0x26];
        n += ((u8 *)table_801aa4e8)[obj->side * 42 + 0x28];
    } else {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + 0x15 + obj->kind];
    }
    write_two_digit_decimal_ascii_slot04_sel(p, n);
    write_two_digit_decimal_ascii_slot04_sel(s2->buf + 0x18, table_801aa4e8[2][0][obj->side]);
    add_item_to_table(s2);
    if (rec->field_04 == 0) {
        if (((game_state.side_mask >> obj->side) & 1) == 0) {
            return;
        }
        data_801b9d34_slot04_sel = rec->field_04;
        set_selection_from_direction_buttons_slot04_sel(obj, rec, 0x11);
        t = data_801b7cc4_slot04_sel[rec->kind_index * 5 + data_801b9d30_slot04_sel];
        rec->kind_index = t;
        data_801b9d30_slot04_sel = t;
        rec->field_04 = data_801b9d34_slot04_sel;
        obj->kind = data_801b9d30_slot04_sel;
        game_state.selection_value = data_801b9d30_slot04_sel;
        update_kind_2_4_hold_counters_b_slot04_sel(obj, rec);
        if (rec->count_2 == 0xb4 && obj->kind == 4) {
            obj->kind = 0x12;
        }
        if (data_8016e68e != 0 && rec->field_14 == 0xb4 && obj->kind == 2) {
            obj->kind = 0x14;
        }
        update_button_sequence_matcher_2_slot04_sel(obj, rec);
        if (rec->field_04 != 0 && obj->kind == 2 && rec->field_12 != 0) {
            obj->kind = 0x14;
            data_8016e68e = -1;
        }
        s5->pos_x = data_801b7c70_slot04_sel[data_801b9d30_slot04_sel].field_00 + obj->side * 0x18;
        s5->pos_y = data_801b7c70_slot04_sel[data_801b9d30_slot04_sel].field_02;
        add_item_to_table(s5);

        i = 0;
        s3 = data_801b7ff4_slot04_sel[obj->side];
        a1 = data_801b82cc_slot04_sel[obj->kind];
        do {
            s3->copied_bytes[i] = a1->copied_bytes[i];
            i++;
        } while (i < 0x10);
        s3->field_0b = a1->field_0b;
        add_item_to_table(s3);

        i = 0;
        s4 = data_801b7fa4_slot04_sel[obj->side];
        b1 = data_801b8978_slot04_sel[obj->kind];
        do {
            s4->text[i] = b1->text[i];
            i++;
        } while (i < 10);
        s4->palette = b1->palette;
        add_item_to_table(s4);

        if (rec->field_04 == 0) {
            return;
        }
        call_func_with_side_x_origin(obj->side, data_801b7d30_slot04_sel[data_801b9d30_slot04_sel]);
        game_state.side = obj->side;
        game_state.mode |= 1 << obj->side;
        if (rec->input_bits == 0x10 || rec->input_bits == 0x80) {
            obj->field_d4 = 0;
            obj->select_value_1 = 0;
        }
        if (rec->input_bits == 0x20 || rec->input_bits == 0x40) {
            obj->field_d4 = 1;
            obj->select_value_1 = 1;
        }
        if (rec->input_bits == 0x90) {
            obj->field_d4 = 2;
            obj->select_value_1 = 2;
        }
        if (rec->input_bits == 0x60) {
            obj->field_d4 = 3;
            obj->select_value_1 = 3;
        }
        if (rec->input_bits == 0xc0) {
            obj->field_d4 = 4;
            obj->select_value_1 = 4;
        }
        if (rec->input_bits == 0x30) {
            obj->field_d4 = 5;
            obj->select_value_1 = 5;
        }
        ref_other.p = obj;
        check_opposite_player_same_selection_2_slot04_sel();
        if (data_8019045c[0] != 0) {
            data_8019045c[0] = 0;
            if (obj->field_d4 == 0) {
                data_8019045c[0] = 1;
            }
            obj->field_d4 = data_8019045c[0];
        }
    } else {
        add_item_to_table(s5);
        add_item_to_table(s3);
        add_item_to_table(s4);
    }
}
