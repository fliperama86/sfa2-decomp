/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04SelRec6d8c data_801b6d8c_slot04_sel[];
extern u8 data_801b6de0_slot04_sel[];
extern u8 data_801b6e4c_slot04_sel[];
extern void (*data_801b6e68_slot04_sel[])(Object *, Slot04SelRec *);
extern Object *data_801b6f1c_slot04_sel[];
extern TextItem *data_801b6fcc_slot04_sel[];
extern TextItem *data_801b701c_slot04_sel[];
extern TextItem *data_801b72f4_slot04_sel[];
extern TextItem *data_801b74cc_slot04_sel[];
extern TextItem *data_801b791c_slot04_sel[];
extern u8 data_801b9d28_slot04_sel;
extern u8 data_801b9cec_slot04_sel;
extern u8 data_801b9ce8_slot04_sel;

void func_801b1984_slot04_sel(void);
void func_801b1d8c_slot04_sel(Object *obj, Slot04SelRec *p, int n);
void func_801b1f20_slot04_sel(Object *obj, Slot04SelRec *p);
void func_801b2008_slot04_sel(Object *obj, Slot04SelRec *p);

void func_801b08b0_slot04_sel(Object *obj, Slot04SelRec *p) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    TextItem *s2 = data_801b701c_slot04_sel[obj->side];
    TextItem *s1 = data_801b6fcc_slot04_sel[obj->side];
    TextItem *s4 = data_801b74cc_slot04_sel[obj->side];
    TextItem *a1;
    int i;

    if (p->field_04 == 0) {
        u8 a0 = game_state.side_mask;
        if ((a0 >> obj->side) & 1) {
            u8 v1 = data_801b9d28_slot04_sel;
            if (v1 != a0) {
                int m = 1 << obj->side;
                if ((v1 & m) != (a0 & m)) {
                    data_801b6e68_slot04_sel[obj->side](obj, p);
                    if (p->kind == 0x12) {
                        p->kind = 4;
                    }
                    if (p->kind == 0x13) {
                        p->kind = 0x11;
                    }
                    if (p->kind == 0x14) {
                        p->kind = 2;
                    }
                }
            }
            data_801b9cec_slot04_sel = p->field_04;
            func_801b1d8c_slot04_sel(obj, p, 0x11);
            p->kind = data_801b6de0_slot04_sel[p->kind * 5 + data_801b9ce8_slot04_sel];
            data_801b9ce8_slot04_sel = p->kind;
            p->field_04 = data_801b9cec_slot04_sel;
            obj->kind = data_801b9ce8_slot04_sel;
            game_state.selection_value = data_801b9ce8_slot04_sel;
            func_801b1f20_slot04_sel(obj, p);
            if (p->field_17 == 0xb4 && obj->kind == 4) {
                obj->kind = 0x12;
            }
            if (data_8016e68e != 0 && p->field_18 == 0xb4 && obj->kind == 2) {
                obj->kind = 0x14;
            }
            func_801b2008_slot04_sel(obj, p);
            if (p->field_04 != 0 && obj->kind == 2 && p->slot_d_flag != 0) {
                obj->kind = 0x14;
                data_8016e68e = -1;
            }
            s4->x = data_801b6d8c_slot04_sel[data_801b9ce8_slot04_sel].field_00 + obj->side * 0x18;
            s4->y = data_801b6d8c_slot04_sel[data_801b9ce8_slot04_sel].field_02;
            func_801519b4(s4);
            s2 = data_801b701c_slot04_sel[obj->side];
            a1 = data_801b72f4_slot04_sel[obj->kind];
            for (i = 0; i < 0x10; i++) {
                s2->text2[i] = a1->text2[i];
            }
            s2->palette = a1->palette;
            func_801519b4(s2);
            s1 = data_801b6fcc_slot04_sel[obj->side];
            a1 = data_801b791c_slot04_sel[obj->kind];
            for (i = 0; i < 10; i++) {
                s1->text[i] = a1->text[i];
            }
            s1->palette = a1->palette;
            func_801519b4(s1);
            if (p->field_04 != 0) {
                func_801205c4(obj->side, data_801b6e4c_slot04_sel[data_801b9ce8_slot04_sel]);
                game_state.side = obj->side;
                game_state.mode |= 1 << obj->side;
                if (p->input_value == 0x10 || p->input_value == 0x80) {
                    obj->field_d4 = 0;
                    obj->select_value_1 = 0;
                }
                if (p->input_value == 0x20 || p->input_value == 0x40) {
                    obj->field_d4 = 1;
                    obj->select_value_1 = 1;
                }
                if (p->input_value == 0x90) {
                    obj->field_d4 = 2;
                    obj->select_value_1 = 2;
                }
                if (p->input_value == 0x60) {
                    obj->field_d4 = 3;
                    obj->select_value_1 = 3;
                }
                if (p->input_value == 0xc0) {
                    obj->field_d4 = 4;
                    obj->select_value_1 = 4;
                }
                if (p->input_value == 0x30) {
                    obj->field_d4 = 5;
                    obj->select_value_1 = 5;
                }
                ref_other.p = obj;
                func_801b1984_slot04_sel();
                if (data_8019045c[0] != 0) {
                    data_8019045c[0] = 0;
                    if (obj->field_d4 == 0) {
                        data_8019045c[0] = 1;
                    }
                    obj->field_d4 = *(u8 *)data_8019045c;
                }
            }
        } else if ((game_state.mode >> obj->side) & 1) {
            a1 = data_801b72f4_slot04_sel[obj->kind];
            for (i = 0; i < 0x10; i++) {
                s2->text2[i] = a1->text2[i];
            }
            s2->palette = a1->palette;
            func_801519b4(s2);
            s1 = data_801b6fcc_slot04_sel[obj->side];
            a1 = data_801b791c_slot04_sel[obj->kind];
            for (i = 0; i < 10; i++) {
                s1->text[i] = a1->text[i];
            }
            s1->palette = a1->palette;
            func_801519b4(s1);
        } else {
            func_801519b4(data_801b6f1c_slot04_sel[obj->side]);
        }
    } else {
        func_801519b4(s4);
        func_801519b4(s2);
        func_801519b4(s1);
    }
}
