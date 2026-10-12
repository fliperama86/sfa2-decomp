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
 * table_index from four consecutive entries of the word table
 * data_801e9f60_slot06_00 (entries 4*n to 4*n+3, n = table_index; the index is
 * taken as a byte): two entries are copied whole, and the other two give a
 * word with the low half cleared and a word that is 0xf80000 minus their
 * low half shifted up by 16. The record also gets pointers into the
 * sequence table data_801e9f48_slot06_00 (the step table_index and the one
 * after it); the object's sequence is set to the first. Then it sets
 * fixed size fields and mirrors pos_y (0xf8 minus its low 16 bits).
 */
void init_object_from_start_table_slot06_00(Object *obj) {
    Slot06_00Rec3010 *r;
    int i;
    /* One local for the three indices; a fresh expression per access gives 51 differing slots (392 bytes). */
    int k;
    int m;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_0c = 0;
    obj->state_0++;
    r = &data_801f3010_slot06_00[obj->table_index];
    /* The original clears the low halves first and overwrites them below; without these four statements: 56 differing slots (376 bytes). */
    r->pos_x_accum &= 0xffff0000;
    r->pos_y_a &= 0xffff0000;
    r->alt_pos_x_accum &= 0xffff0000;
    r->pos_y_b &= 0xffff0000;
    i = obj->table_index << 2;
    r->step_x_a = data_801e9f60_slot06_00[(u8)i];
    k = i | 1;
    r->pos_x_accum = data_801e9f60_slot06_00[k & 0xff] & 0xffff0000;
    /* Stand-in, not recovered source: the local m, which is only a copy of k, reproduces the original's order of instructions. Without it this function differs from the original in 6 instruction slots. */
    /* The figure's control keeps the value: m is gone and the access that reads it has the index (i | 1) & 0xff; the build is then 428 bytes. */
    m = k;
    k = i | 2;
    r->pos_y_a = 0xf80000 - ((u16)data_801e9f60_slot06_00[m & 0xff] << 16);
    r->step_x_b = data_801e9f60_slot06_00[(u8)k];
    k = i | 3;
    r->alt_pos_x_accum = data_801e9f60_slot06_00[k & 0xff] & 0xffff0000;
    r->pos_y_b = 0xf80000 - ((u16)data_801e9f60_slot06_00[k & 0xff] << 16);
    /* The original stores this pointer twice; with one store: 10 differing slots (436 bytes). */
    r->sequence_start = data_801e9f48_slot06_00;
    r->sequence_start = data_801e9f48_slot06_00 + obj->table_index;
    r->sequence_next = r->sequence_start + 1;
    obj->sequence = r->sequence_start;
    obj->base_x = 0x280;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->spawn_mode = 4;
    obj->image_row = 0;
    obj->pos_y = 0xf8 - (u16)obj->pos_y;
}
