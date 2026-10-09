/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

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
extern s8 data_8016e68e;

void func_801b2aa4_slot04_sel(u8 *dst, u8 n);
void func_801b310c_slot04_sel(Object *obj, Slot04SelRec9d38 *rec);
void func_801b31f4_slot04_sel(Object *obj, Slot04SelRec9d38 *rec);
void func_801b48f4_slot04_sel(Object *obj, Slot04SelRec9d38 *rec, int a);
void func_801b44ec_slot04_sel(void);

void func_801b2b24_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
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
    func_801b2aa4_slot04_sel(p, n);
    p = s2->buf + 0xf;
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + 0x26];
        n += ((u8 *)table_801aa4e8)[obj->side * 42 + 0x28];
    } else {
        n = ((u8 *)table_801aa4e8)[obj->side * 42 + 0x15 + obj->kind];
    }
    func_801b2aa4_slot04_sel(p, n);
    func_801b2aa4_slot04_sel(s2->buf + 0x18, table_801aa4e8[2][0][obj->side]);
    func_801519b4((Object *)s2);
    if (rec->field_04 == 0) {
        if (((game_state.field_07 >> obj->side) & 1) == 0) {
            return;
        }
        data_801b9d34_slot04_sel = rec->field_04;
        func_801b48f4_slot04_sel(obj, rec, 0x11);
        t = data_801b7cc4_slot04_sel[rec->field_03 * 5 + data_801b9d30_slot04_sel];
        rec->field_03 = t;
        data_801b9d30_slot04_sel = t;
        rec->field_04 = data_801b9d34_slot04_sel;
        obj->kind = data_801b9d30_slot04_sel;
        game_state.field_b1 = data_801b9d30_slot04_sel;
        func_801b310c_slot04_sel(obj, rec);
        if (rec->field_13 == 0xb4 && obj->kind == 4) {
            obj->kind = 0x12;
        }
        if (data_8016e68e != 0 && rec->field_14 == 0xb4 && obj->kind == 2) {
            obj->kind = 0x14;
        }
        func_801b31f4_slot04_sel(obj, rec);
        if (rec->field_04 != 0 && obj->kind == 2 && rec->field_12 != 0) {
            obj->kind = 0x14;
            data_8016e68e = -1;
        }
        s5->field_04 = data_801b7c70_slot04_sel[data_801b9d30_slot04_sel].field_00 + obj->side * 0x18;
        s5->field_06 = data_801b7c70_slot04_sel[data_801b9d30_slot04_sel].field_02;
        func_801519b4((Object *)s5);

        i = 0;
        s3 = data_801b7ff4_slot04_sel[obj->side];
        a1 = data_801b82cc_slot04_sel[obj->kind];
        do {
            s3->field_10[i] = a1->field_10[i];
            i++;
        } while (i < 0x10);
        s3->field_0b = a1->field_0b;
        func_801519b4((Object *)s3);

        i = 0;
        s4 = data_801b7fa4_slot04_sel[obj->side];
        b1 = data_801b8978_slot04_sel[obj->kind];
        do {
            s4->field_0c[i] = b1->text[i];
            i++;
        } while (i < 10);
        s4->field_0b = b1->field_0b;
        func_801519b4((Object *)s4);

        if (rec->field_04 == 0) {
            return;
        }
        func_801205c4(obj->side, data_801b7d30_slot04_sel[data_801b9d30_slot04_sel]);
        game_state.field_71 = obj->side;
        game_state.mode |= 1 << obj->side;
        if (rec->field_05 == 0x10 || rec->field_05 == 0x80) {
            obj->field_d4 = 0;
            obj->field_119 = 0;
        }
        if (rec->field_05 == 0x20 || rec->field_05 == 0x40) {
            obj->field_d4 = 1;
            obj->field_119 = 1;
        }
        if (rec->field_05 == 0x90) {
            obj->field_d4 = 2;
            obj->field_119 = 2;
        }
        if (rec->field_05 == 0x60) {
            obj->field_d4 = 3;
            obj->field_119 = 3;
        }
        if (rec->field_05 == 0xc0) {
            obj->field_d4 = 4;
            obj->field_119 = 4;
        }
        if (rec->field_05 == 0x30) {
            obj->field_d4 = 5;
            obj->field_119 = 5;
        }
        ref_other.p = obj;
        func_801b44ec_slot04_sel();
        if (data_8019045c[0] != 0) {
            data_8019045c[0] = 0;
            if (obj->field_d4 == 0) {
                data_8019045c[0] = 1;
            }
            obj->field_d4 = data_8019045c[0];
        }
    } else {
        func_801519b4((Object *)s5);
        func_801519b4((Object *)s3);
        func_801519b4((Object *)s4);
    }
}
