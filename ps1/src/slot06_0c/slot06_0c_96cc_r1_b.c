/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e988c_slot06_0c(Slot06Pal *pal);
void func_801e9838_slot06_0c(Slot06Cursor *cur);
void func_801e98c8_slot06_0c(Slot06Pal *pal, SeqRec *rec);

void func_801e97c8_slot06_0c(Object *obj) {
    if (game_state.field_74 == 0) {
        func_801e9838_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x5c));
        func_801e9838_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x70));
        func_801e9838_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x4c));
    }
    func_80120028(obj);
}

void func_801e9818_slot06_0c(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9838_slot06_0c(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e98c8_slot06_0c((Slot06Pal *)cur, rec);
    }
}

void func_801e988c_slot06_0c(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e98c8_slot06_0c(pal, (SeqRec *)table_8017c910[pal->field_0c]);
}
