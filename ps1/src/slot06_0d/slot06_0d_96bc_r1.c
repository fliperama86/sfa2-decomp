/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea520_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

void func_801e9828_slot06_0d(Object *obj);
void func_801e98bc_slot06_0d(Object *obj);
void func_801e98fc_slot06_0d(Slot06Cursor *cur);
void func_801e9950_slot06_0d(Slot06Pal *pal);
void func_801e998c_slot06_0d(Slot06Pal *pal, SeqRec *rec);

void func_801e96bc_slot06_0d(Object *obj) {
    data_801ea520_slot06_0d[obj->field_04](obj);
}

void func_801e96fc_slot06_0d(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    int i = 1;
    o->field_81 = 4;
    o->field_68 = 0xd;
    obj->field_64 = 0x1f0;
    o->field_04 = o->field_04 + 1;
    o->field_01 = 0;
    o->field_0d = 0;
    o->field_0c = 0;
    o->field_0a = i;
    o->field_0f = i;
    obj->field_66 = 0x80;
    o->field_04 = i;
    func_801e9950_slot06_0d((Slot06Pal *)((u8 *)o + 0x5c));
    i = game_state.field_42 ? 0xa : 3;
    func_80130700(o, data_801eb1b0_slot06_0d[i]);
}

void func_801e97a4_slot06_0d(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            if (obj->field_05 == 0) {
                func_801e9828_slot06_0d(obj);
            } else if (obj->field_05 == 1) {
                func_801e98bc_slot06_0d(obj);
            }
            func_801e98fc_slot06_0d((Slot06Cursor *)((u8 *)obj + 0x5c));
        }
    }
    func_80120028(obj);
}

void func_801e9828_slot06_0d(Object *obj) {
    int i;
    if (game_state.field_47 != 0 && game_state.field_5c == 0) {
        obj->field_05 = obj->field_05 + 1;
        i = game_state.field_78->kind != 0xd ? 0xb : 0xc;
        func_80130700(obj, data_801eb1b0_slot06_0d[i]);
    } else {
        func_80131094(obj);
    }
}

void func_801e98bc_slot06_0d(Object *obj) {
    func_80131094(obj);
}

void func_801e98dc_slot06_0d(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e98fc_slot06_0d(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e998c_slot06_0d((Slot06Pal *)cur, rec);
    }
}

void func_801e9950_slot06_0d(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e998c_slot06_0d(pal, (SeqRec *)table_8017c910[pal->field_0c]);
}
