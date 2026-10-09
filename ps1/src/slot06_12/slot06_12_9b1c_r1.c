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

void func_801e9b1c_slot06_12(Object *obj);
void func_801e9be8_slot06_12(Object *obj);
void func_801e9db4_slot06_12(Object *obj);
void func_801e9e0c_slot06_12(Object *obj);
void func_801e9e68_slot06_12(Object *obj, SeqRec *rec);

void func_801e9b1c_slot06_12(Object *obj) {
    s16 r;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    s16 *c = &data_801904cc;
    int i = (u16)*c & 3;

    obj->field_1c = *(u16 *)((u8 *)data_801ef310_slot06_12 + (i << 2));
    obj->pos_y = 0xf8 - *(u16 *)((u8 *)data_801ef310_slot06_12 + (i << 2) + 2);
    r = func_80151184() & 0xff;
    obj->pos_x = 0xc0;
    if (*c != 3) {
        obj->pos_x = r + 0x40;
    }
    func_80130768(obj, data_801ef320_slot06_12[(*c << 2) + (r & 3)], data_801eee74_slot06_12);
}

void func_801e9be8_slot06_12(Object *obj) {
    s16 *c = &data_801904cc;

    *c = 0;
    *c = (obj->field_54 >> 8) & 0x7f;
    obj->field_03 = *(u8 *)c;
}

void func_801e9c14_slot06_12(Object *obj) {
    if (game_state.field_65 == 0) {
        func_801e9db4_slot06_12(obj);
        data_801ef330_slot06_12[obj->field_03](obj);
    } else {
        obj->field_01 = 0;
    }
}

void func_801e9c80_slot06_12(Object *obj) {
    func_801e9be8_slot06_12(obj);
    if (data_801904cc != 0) {
        func_801e9b1c_slot06_12(obj);
        func_80120028(obj);
    }
}

void func_801e9ccc_slot06_12(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801e9be8_slot06_12(obj);
        func_801e9b1c_slot06_12(obj);
    }
    func_80131094(obj);
    if ((s16)obj->field_3a != 0) {
        obj->pos_x += 8;
    }
    func_80120028(obj);
}

void func_801e9d40_slot06_12(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        func_801e9be8_slot06_12(obj);
        func_801e9b1c_slot06_12(obj);
    }
    func_80131094(obj);
    func_80120028(obj);
}

void func_801e9d94_slot06_12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9db4_slot06_12(Object *obj) {
    SeqRec *rec;

    obj->field_50 = obj->field_50 - 1;
    if (obj->field_50 == 0) {
        rec = (SeqRec *)obj->field_4c;
        rec++;
        if (obj->field_54 < 0) {
            func_801e9e0c_slot06_12(obj);
        } else {
            func_801e9e68_slot06_12(obj, rec);
        }
    }
}

void func_801e9e0c_slot06_12(Object *obj) {
    obj->field_58 = (obj->field_58 + 1) & 0x3f;
    func_801e9e68_slot06_12(obj, data_801ef2f4_slot06_12[data_801eef00_slot06_12[obj->field_58]]);
}

void func_801e9e68_slot06_12(Object *obj, SeqRec *rec) {
    int w;

    obj->field_4c = (s32)rec;
    w = rec->header;
    obj->field_50 = w >> 16;
    obj->field_54 = (s16)w;
    func_8011fcc0(rec);
}
