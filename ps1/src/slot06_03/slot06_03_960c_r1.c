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

void func_801e9790_slot06_03(Object *obj);
void func_801e9810_slot06_03(Object *obj);
void func_801e9870_slot06_03(Object *obj);
void func_801e98e0_slot06_03(Object *obj);
void func_801e9a2c_slot06_03(SeqRec *rec, Slot06Cursor *cur);
void func_801e9af0_slot06_03(Slot06Cursor *cur);
void func_801e9b44_slot06_03(Slot06Cursor *cur, SeqRec *rec);

void func_801e960c_slot06_03(Object *obj) {
    data_801ea5f4_slot06_03[obj->field_04](obj);
}

void func_801e964c_slot06_03(Object *obj) {
    obj->field_0f = 1;
    obj->field_76 = 0x2c0;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->field_04 = obj->field_04 + 1;
    func_801e9790_slot06_03(obj);
    func_801e9810_slot06_03(obj);
    func_801e9870_slot06_03(obj);
}

void func_801e96cc_slot06_03(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        *(u32 *)&obj->field_10 += obj->field_4c;
        *(u32 *)&obj->field_14 += obj->field_50;
        obj->field_4c += obj->field_54;
        if (obj->pos_y < 0) {
            func_801e9790_slot06_03(obj);
            func_801e9810_slot06_03(obj);
            func_801e9870_slot06_03(obj);
            func_801e98e0_slot06_03(obj);
        }
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9770_slot06_03(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9790_slot06_03(Object *obj) {
    u8 r;
    obj->field_50 = -*(s32 *)(data_801ea604_slot06_03 + ((obj->field_03 << 3) & 0x3f8));
    r = func_80151184() & 1;
    if (r != 0) {
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    obj->field_0b = r;
}

void func_801e9810_slot06_03(Object *obj) {
    obj->pos_x = data_801ea61c_slot06_03[(u8)(obj->field_03 * 16 + (func_80151184() & 0xf) * 2)];
    obj->pos_y = 0xe0;
}

void func_801e9870_slot06_03(Object *obj) {
    u8 v = data_801ea67c_slot06_03[func_80151184() & 3];
    obj->field_48 = v;
    func_80130700(obj, data_801eb3e0_slot06_03[(u8)(obj->field_03 * 3 + v)]);
}

void func_801e98e0_slot06_03(Object *obj) {
    obj->field_1c = data_801ea680_slot06_03[(u8)((func_80151184() & 3) | (obj->field_48 << 2))];
}

void func_801e9938_slot06_03(Object *obj) {
    data_801ea698_slot06_03[obj->field_04](obj);
}

void func_801e9978_slot06_03(Object *obj) {
    obj->field_01 = 0;
    obj->field_04 = obj->field_04 + 1;
    func_801e9a2c_slot06_03(data_801ea2cc_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x3c));
    func_801e9a2c_slot06_03(data_801ea310_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x10));
    func_801e9a2c_slot06_03(data_801ea324_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x18));
    func_801e9a2c_slot06_03(data_801ea418_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x28));
    func_801e9a2c_slot06_03(data_801ea53c_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x30));
    func_801e9a2c_slot06_03(data_801ea560_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x4c));
    func_801e9a2c_slot06_03(data_801ea294_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x54));
    func_801e9a2c_slot06_03(data_801ea2a8_slot06_03, (Slot06Cursor *)((u8 *)obj + 0x6c));
}

void func_801e9a2c_slot06_03(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
}

void func_801e9a4c_slot06_03(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x3c));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x10));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x18));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x28));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x30));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x4c));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x54));
            func_801e9af0_slot06_03((Slot06Cursor *)((u8 *)obj + 0x6c));
        }
    }
}

void func_801e9ad0_slot06_03(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9af0_slot06_03(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e9b44_slot06_03(cur, rec);
    }
}

void func_801e9b44_slot06_03(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
    func_8011fcc0(rec);
}

void func_801e9b80_slot06_03(Object *obj) {
    data_801ea6a8_slot06_03[obj->field_04](obj);
}

void func_801e9bc0_slot06_03(Object *obj) {
    obj->field_0f = 1;
    obj->field_76 = 0x2c0;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_81 = 4;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_04 = obj->field_04 + 1;
    func_80130700(obj, data_801eb404_slot06_03[obj->field_03]);
}

void func_801e9c40_slot06_03(Object *obj) {
    int a = (game_state.field_32 & 1) << 2;
    int i = (u8)(obj->field_03 << 3 | a);
    obj->pos_x = data_801ea6b8_slot06_03[i];
    obj->pos_y = 0xf8 - data_801ea6b8_slot06_03[i + 1];
    obj->field_0b = ((u8 *)data_801ea6b8_slot06_03)[i * 2 + 4];
    if (a != 0) {
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9ce4_slot06_03(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
