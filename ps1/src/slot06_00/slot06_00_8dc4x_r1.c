/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e9f48_slot06_00;
extern u32 data_801e9f60_slot06_00[];
extern Slot06_00Rec3010 data_801f3010_slot06_00[];

/*
 * What it does (inferred, not an original name): initializes an object for
 * its first frame. It sets a few flag bytes, counts up state_0, and fills
 * the 0x20-byte record of data_801f3010_slot06_00 selected by the object's
 * field_03 from four consecutive entries of the word table
 * data_801e9f60_slot06_00 (entries 4*n to 4*n+3, n = field_03; the index is
 * taken as a byte): two entries are copied whole, and the other two give a
 * word with the low half cleared and a word that is 0xf80000 minus their
 * low half shifted up by 16. The record also gets pointers into the
 * sequence table data_801e9f48_slot06_00 (the step field_03 and the one
 * after it); the object's sequence is set to the first. Then it sets
 * fixed size fields and mirrors pos_y (0xf8 minus its low 16 bits).
 */
void func_801e8dc4_slot06_00(Object *obj) {
    Slot06_00Rec3010 *r;
    int i;
    /* One local for the three indices; a fresh expression per access gives 51 differing slots (392 bytes). */
    int k;
    int m;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->state_0++;
    r = &data_801f3010_slot06_00[obj->field_03];
    /* The original clears the low halves first and overwrites them below; without these four statements: 56 differing slots (376 bytes). */
    r->field_00 &= 0xffff0000;
    r->field_04 &= 0xffff0000;
    r->field_08 &= 0xffff0000;
    r->field_0c &= 0xffff0000;
    i = obj->field_03 << 2;
    r->field_10 = data_801e9f60_slot06_00[(u8)i];
    k = i | 1;
    r->field_00 = data_801e9f60_slot06_00[k & 0xff] & 0xffff0000;
    /* Stand-in, not recovered source: the local m, which is only a copy of k, reproduces the original's order of instructions. Without it this function differs from the original in 34 instruction slots. */
    m = k;
    k = i | 2;
    r->field_04 = 0xf80000 - ((u16)data_801e9f60_slot06_00[m & 0xff] << 16);
    r->field_14 = data_801e9f60_slot06_00[(u8)k];
    k = i | 3;
    r->field_08 = data_801e9f60_slot06_00[k & 0xff] & 0xffff0000;
    r->field_0c = 0xf80000 - ((u16)data_801e9f60_slot06_00[k & 0xff] << 16);
    /* The original stores this pointer twice; with one store: 10 differing slots (436 bytes). */
    r->field_18 = data_801e9f48_slot06_00;
    r->field_18 = data_801e9f48_slot06_00 + obj->field_03;
    r->field_1c = r->field_18 + 1;
    obj->sequence = r->field_18;
    obj->field_76 = 0x280;
    obj->field_78 = 0x100;
    obj->field_7a = 0x70;
    obj->field_7c = 0x1e0;
    obj->field_81 = 4;
    obj->field_0d = 0;
    obj->pos_y = 0xf8 - (u16)obj->pos_y;
}
