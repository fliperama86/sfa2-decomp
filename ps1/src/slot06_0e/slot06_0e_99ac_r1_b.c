/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9bc8_slot06_0e(Object *obj);
void func_801e9bf0_slot06_0e(Slot06Cursor *cur);
void func_801e9c44_slot06_0e(Slot06Pal *pal);
void func_801e9c80_slot06_0e(Slot06Pal *pal, SeqRec *rec);

void func_801e9bc8_slot06_0e(Object *obj) {
}

void func_801e9bd0_slot06_0e(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9bf0_slot06_0e(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->field_04 - 1;
    cur->field_04 = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->field_06 < 0) {
            rec = (SeqRec *)rec->header;
        }
        func_801e9c80_slot06_0e((Slot06Pal *)cur, rec);
    }
}

void func_801e9c44_slot06_0e(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    func_801e9c80_slot06_0e(pal, (SeqRec *)table_8017c910[pal->field_0c]);
}
