/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea958_slot06_10[];
extern SeqRec data_801ea868_slot06_10[];
extern SeqRec data_801ea8a0_slot06_10[];
extern SeqRec data_801ea8d8_slot06_10[];

void func_801e8f14_slot06_10(SeqRec *rec, Slot06Cursor *cur);
void func_801e9018_slot06_10(Slot06Pal *pal);
void func_801e906c_slot06_10(Slot06Pal *pal);
void func_801e90a8_slot06_10(Slot06Pal *pal, SeqRec *rec);
void func_801e913c_slot06_10(Slot06Cursor *cur);

void func_801e8e18_slot06_10(Object *obj) {
    data_801ea958_slot06_10[obj->field_04](obj);
}

void func_801e8e58_slot06_10(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    obj->field_3c = 0x10;
    obj->field_4c = 0x11;
    obj->field_5c = 0x12;
    obj->field_38 = 0x2e0;
    obj->field_48 = 0x200;
    obj->field_58 = 0x180;
    obj->field_01 = 0;
    obj->field_3a = 0x50;
    obj->field_4a = 0x50;
    obj->field_5a = 0x50;
    obj->field_04 = obj->field_04 + 1;
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
    func_801e906c_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
    func_801e8f14_slot06_10(data_801ea868_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x28));
    func_801e8f14_slot06_10(data_801ea8a0_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x6c));
    func_801e8f14_slot06_10(data_801ea8d8_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x84));
}

void func_801e8f14_slot06_10(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->field_04 = new_var;
    cur->field_06 = w;
}

void func_801e8f34_slot06_10(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
            if (((Slot06Obj *)obj)->field_46 != 0) {
                ((Slot06Obj *)obj)->field_12 = 0x340;
                ((Slot06Obj *)obj)->field_16 = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_46 = *(u16 *)&((Slot06Obj *)obj)->field_46 & 0xff00;
            }
            func_801e9018_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
            if (((Slot06Obj *)obj)->field_56 != 0) {
                ((Slot06Obj *)obj)->field_12 = 0x1c0;
                ((Slot06Obj *)obj)->field_16 = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_56 = *(u16 *)&((Slot06Obj *)obj)->field_56 & 0xff00;
            }
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x28));
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x6c));
            func_801e913c_slot06_10((Slot06Cursor *)((u8 *)obj + 0x84));
        }
    }
}

void func_801e8ff8_slot06_10(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9018_slot06_10(Slot06Pal *pal) {
    SeqRec *rec;
    int v = pal->field_04 - 1;
    pal->field_04 = v;
    if ((s16)v == 0) {
        rec = pal->rec;
        rec++;
        if ((s16)pal->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e90a8_slot06_10(pal, rec);
    }
}

void func_801e906c_slot06_10(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e90a8_slot06_10(pal, (SeqRec *)table_8017c910[pal->field_0c]);
}
