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
extern s8 data_8016e68e;

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
        u8 a0 = game_state.field_07;
        if ((a0 >> obj->side) & 1) {
            u8 v1 = data_801b9d28_slot04_sel;
            if (v1 != a0) {
                int m = 1 << obj->side;
                if ((v1 & m) != (a0 & m)) {
                    data_801b6e68_slot04_sel[obj->side](obj, p);
                    if (p->field_03 == 0x12) {
                        p->field_03 = 4;
                    }
                    if (p->field_03 == 0x13) {
                        p->field_03 = 0x11;
                    }
                    if (p->field_03 == 0x14) {
                        p->field_03 = 2;
                    }
                }
            }
            data_801b9cec_slot04_sel = p->field_04;
            func_801b1d8c_slot04_sel(obj, p, 0x11);
            p->field_03 = data_801b6de0_slot04_sel[p->field_03 * 5 + data_801b9ce8_slot04_sel];
            data_801b9ce8_slot04_sel = p->field_03;
            p->field_04 = data_801b9cec_slot04_sel;
            obj->kind = data_801b9ce8_slot04_sel;
            game_state.field_b1 = data_801b9ce8_slot04_sel;
            func_801b1f20_slot04_sel(obj, p);
            if (p->field_17 == 0xb4 && obj->kind == 4) {
                obj->kind = 0x12;
            }
            if (data_8016e68e != 0 && p->field_18 == 0xb4 && obj->kind == 2) {
                obj->kind = 0x14;
            }
            func_801b2008_slot04_sel(obj, p);
            if (p->field_04 != 0 && obj->kind == 2 && p->field_16 != 0) {
                obj->kind = 0x14;
                data_8016e68e = -1;
            }
            s4->field_04 = data_801b6d8c_slot04_sel[data_801b9ce8_slot04_sel].field_00 + obj->side * 0x18;
            s4->field_06 = data_801b6d8c_slot04_sel[data_801b9ce8_slot04_sel].field_02;
            func_801519b4((Object *)s4);
            s2 = data_801b701c_slot04_sel[obj->side];
            a1 = data_801b72f4_slot04_sel[obj->kind];
            for (i = 0; i < 0x10; i++) {
                s2->text2[i] = a1->text2[i];
            }
            s2->field_0b = a1->field_0b;
            func_801519b4((Object *)s2);
            s1 = data_801b6fcc_slot04_sel[obj->side];
            a1 = data_801b791c_slot04_sel[obj->kind];
            for (i = 0; i < 10; i++) {
                s1->text[i] = a1->text[i];
            }
            s1->field_0b = a1->field_0b;
            func_801519b4((Object *)s1);
            if (p->field_04 != 0) {
                func_801205c4(obj->side, data_801b6e4c_slot04_sel[data_801b9ce8_slot04_sel]);
                game_state.field_71 = obj->side;
                game_state.mode |= 1 << obj->side;
                if (p->field_05 == 0x10 || p->field_05 == 0x80) {
                    obj->field_d4 = 0;
                    obj->field_119 = 0;
                }
                if (p->field_05 == 0x20 || p->field_05 == 0x40) {
                    obj->field_d4 = 1;
                    obj->field_119 = 1;
                }
                if (p->field_05 == 0x90) {
                    obj->field_d4 = 2;
                    obj->field_119 = 2;
                }
                if (p->field_05 == 0x60) {
                    obj->field_d4 = 3;
                    obj->field_119 = 3;
                }
                if (p->field_05 == 0xc0) {
                    obj->field_d4 = 4;
                    obj->field_119 = 4;
                }
                if (p->field_05 == 0x30) {
                    obj->field_d4 = 5;
                    obj->field_119 = 5;
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
            s2->field_0b = a1->field_0b;
            func_801519b4((Object *)s2);
            s1 = data_801b6fcc_slot04_sel[obj->side];
            a1 = data_801b791c_slot04_sel[obj->kind];
            for (i = 0; i < 10; i++) {
                s1->text[i] = a1->text[i];
            }
            s1->field_0b = a1->field_0b;
            func_801519b4((Object *)s1);
        } else {
            func_801519b4(data_801b6f1c_slot04_sel[obj->side]);
        }
    } else {
        func_801519b4((Object *)s4);
        func_801519b4((Object *)s2);
        func_801519b4((Object *)s1);
    }
}
