/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eaec0_slot06_07[];

void func_801e984c_slot06_07(Slot06Pal *pal);
void func_801e98a0_slot06_07(Slot06Pal *pal);
void func_801e98dc_slot06_07(Slot06Pal *pal, SeqRec *rec);

void func_801e96c0_slot06_07(Object *obj) {
    data_801eaec0_slot06_07[obj->field_04](obj);
}

void func_801e9700_slot06_07(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Slot06Pal *p0 = (Slot06Pal *)((u8 *)obj + 0x30);
    Slot06Pal *p1 = (Slot06Pal *)((u8 *)obj + 0x40);
    Slot06Pal *p2 = (Slot06Pal *)((u8 *)obj + 0x50);
    Slot06Pal *p3 = (Slot06Pal *)((u8 *)obj + 0x6c);
    obj->field_3c = 7;
    obj->field_4c = 8;
    obj->field_5c = 9;
    obj->field_78 = 10;
    obj->field_38 = 0x110;
    obj->field_48 = 0x210;
    obj->field_58 = 0x290;
    obj->field_74 = 0x300;
    obj->field_01 = 0;
    obj->field_3a = 0;
    obj->field_4a = 0x60;
    obj->field_5a = 0x70;
    obj->field_76 = 0x60;
    obj->field_04 = obj->field_04 + 1;
    func_801e98a0_slot06_07(p0);
    func_801e98a0_slot06_07(p1);
    func_801e98a0_slot06_07(p2);
    func_801e98a0_slot06_07(p3);
}

void func_801e97c8_slot06_07(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            func_801e984c_slot06_07((Slot06Pal *)((u8 *)obj + 0x30));
            func_801e984c_slot06_07((Slot06Pal *)((u8 *)obj + 0x40));
            func_801e984c_slot06_07((Slot06Pal *)((u8 *)obj + 0x50));
            func_801e984c_slot06_07((Slot06Pal *)((u8 *)obj + 0x6c));
        }
    }
}

void func_801e982c_slot06_07(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e984c_slot06_07(Slot06Pal *pal) {
    SeqRec *rec;
    int v = pal->field_04 - 1;
    pal->field_04 = v;
    if ((s16)v == 0) {
        rec = pal->rec;
        rec++;
        if ((s16)pal->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e98dc_slot06_07(pal, rec);
    }
}

void func_801e98a0_slot06_07(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e98dc_slot06_07(pal, (SeqRec *)table_8017c910[pal->field_0c]);
}
