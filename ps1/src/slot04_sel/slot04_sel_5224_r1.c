/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Slot04SelRec8fc8 *data_801b8fc8_slot04_sel[];
extern Slot04SelRec8f78 *data_801b8f78_slot04_sel[];
extern Slot04SelRec94c0 *data_801b94c0_slot04_sel[];
extern Slot04SelRec8fc8 *data_801b92a0_slot04_sel[];
extern TextItem *data_801b9908_slot04_sel[];
extern Slot04SelRec8ccc data_801b8ccc_slot04_sel[];
extern u8 data_801b8d20_slot04_sel[];
extern u8 data_801b8d8c_slot04_sel[];
extern u8 data_801b9d70_slot04_sel;
extern u8 data_801b9d74_slot04_sel;

void func_801b64c0_slot04_sel(Object *obj, Slot04SelRec9d78 *rec, int a);
void func_801b56e0_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *rec);
void func_801b57c8_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *rec);
void func_801b60b8_slot04_sel(void);

void func_801b5224_slot04_sel(Object *obj, Object *other, Slot04SelRec9d78 *rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *p;
    u8 side;
    Slot04SelRec8fc8 *s0;
    Slot04SelRec8f78 *s2;
    Slot04SelRec94c0 *s5;
    Slot04SelRec8fc8 *a1;
    TextItem *b1;
    int i;

    if (data_8018f5a0->field_50 == 1) {
        side = obj->side;
        p = obj;
    } else if (data_8018f5a0->field_50 == 2) {
        p = other;
        side = obj->side ^ 1;
    }
    s0 = data_801b8fc8_slot04_sel[side];
    s2 = data_801b8f78_slot04_sel[side];
    s5 = data_801b94c0_slot04_sel[side];
    if (rec->field_04 == 0) {
        if (((game_state.field_07 >> obj->side) & 1) == 0) {
            return;
        }
        data_801b9d74_slot04_sel = rec->field_04;
        func_801b64c0_slot04_sel(obj, rec, 0x11);
        rec->field_03 = data_801b8d20_slot04_sel[rec->field_03 * 5 + data_801b9d70_slot04_sel];
        data_801b9d70_slot04_sel = rec->field_03;
        rec->field_04 = data_801b9d74_slot04_sel;
        p->kind = data_801b9d70_slot04_sel;
        game_state.field_b1 = data_801b9d70_slot04_sel;
        func_801b56e0_slot04_sel(p, obj, rec);
        if (rec->field_14 == 0xb4 && p->kind == 4) {
            p->kind = 0x12;
        }
        if (data_8016e68e != 0) {
            if (rec->field_15 == 0xb4 && p->kind == 2) {
                p->kind = 0x14;
            }
        }
        func_801b57c8_slot04_sel(p, obj, rec);
        if (rec->field_04 != 0 && p->kind == 2 && rec->field_13 != 0) {
            p->kind = 0x14;
            data_8016e68e = -1;
        }
        s5->field_04 = data_801b8ccc_slot04_sel[data_801b9d70_slot04_sel].field_00 + side * 0x18;
        s5->field_06 = data_801b8ccc_slot04_sel[data_801b9d70_slot04_sel].field_02;
        func_801519b4(s5);

        i = 0;
        s0 = data_801b8fc8_slot04_sel[side];
        a1 = data_801b92a0_slot04_sel[p->kind];
        do {
            s0->field_10[i] = a1->field_10[i];
            i++;
        } while (i < 0x10);
        s0->field_0b = a1->field_0b;
        func_801519b4(s0);

        i = 0;
        s2 = data_801b8f78_slot04_sel[side];
        b1 = data_801b9908_slot04_sel[p->kind];
        do {
            s2->field_0c[i] = b1->text[i];
            i++;
        } while (i < 10);
        s2->field_0b = b1->field_0b;
        func_801519b4(s2);

        if (rec->field_04 == 0) {
            return;
        }
        func_801205c4(side, data_801b8d8c_slot04_sel[data_801b9d70_slot04_sel]);
        game_state.field_71 = p->side;
        game_state.mode |= 1 << obj->side;
        if (rec->field_05 == 0x10 || rec->field_05 == 0x80) {
            p->field_d4 = 0;
            p->field_119 = 0;
        }
        if (rec->field_05 == 0x20 || rec->field_05 == 0x40) {
            p->field_d4 = 1;
            p->field_119 = 1;
        }
        if (rec->field_05 == 0x90) {
            p->field_d4 = 2;
            p->field_119 = 2;
        }
        if (rec->field_05 == 0x60) {
            p->field_d4 = 3;
            p->field_119 = 3;
        }
        if (rec->field_05 == 0xc0) {
            p->field_d4 = 4;
            p->field_119 = 4;
        }
        if (rec->field_05 == 0x30) {
            p->field_d4 = 5;
            p->field_119 = 5;
        }
        ref_other.p = p;
        func_801b60b8_slot04_sel();
        if (data_8019045c[0] != 0) {
            data_8019045c[0] = 0;
            if (p->field_d4 == 0) {
                data_8019045c[0] = 1;
            }
            p->field_d4 = data_8019045c[0];
        }
    } else {
        func_801519b4(s5);
        func_801519b4(s0);
        func_801519b4(s2);
    }
}
