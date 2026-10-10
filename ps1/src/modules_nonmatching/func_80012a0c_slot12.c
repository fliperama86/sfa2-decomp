/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 316 bytes against the original's 336.
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test
 * next to it (difftest.py, with func_80012a0c_slot12.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): sets up 14 pairs of
 * 16-byte rectangle records, 0x20 records apart, in one of two record
 * tables (the second table when the object's field_03 is not 0, else the
 * first). For each pair it calls the library on both records (one routine
 * to initialize, one to set a flag, called with 0), clears the colour
 * bytes and sets position (the object's position plus an offset pair from
 * a table of 14 pairs of halfwords), width 0xc0 and height 0x10. Both
 * records of a pair get the same position.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (0x394 bytes in the test).
 *   No return value.
 *   Reads: the object's field_03 (byte), pos_x and pos_y (halfwords at
 *     0x12 and 0x16); the 28 halfwords of data_80022e48_slot12.
 *   Writes: in each of the 28 records touched, the bytes r, g, b and the
 *     halfwords x, y, w, h (offsets 4 to 6 and 8 to 0xf). Not the tag word.
 *   Callees, both in Sony's library, replaced by recorders:
 *     func_8015c150 (1 argument, the record) and func_8015bfe8 (2
 *     arguments, the record and 0). Their results are unused. The log
 *     watches, at every call, the 14 records of each of the two runs of the
 *     chosen table (56 words each), so that a record field written after a
 *     call that the original writes before it is a difference.
 *   Aliasing: the object, the offset table and the record tables are
 *     distinct.
 *   Not reached: nothing is excluded; every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80022e48_slot12[];
extern Slot12Tile data_8002a394_slot12[];
extern Slot12Tile data_8002a794_slot12[];

/* Inferred from the callers' use; not original declarations. */
void func_8015c150(u8 *record);
void func_8015bfe8(Poly28 *quad, int flag);

void func_80012a0c_slot12(Object *obj) {
    Slot12Tile *a;
    Slot12Tile *b;
    u16 *offsets = data_80022e48_slot12;
    int i;

    if (obj->field_03 == 0) {
        a = data_8002a394_slot12;
    } else {
        a = data_8002a794_slot12;
    }
    for (i = 0; i < 14; i++) {
        b = a + 0x20;
        func_8015c150((u8 *)a);
        func_8015c150((u8 *)b);
        func_8015bfe8((Poly28 *)a, 0);
        func_8015bfe8((Poly28 *)b, 0);
        a->r = 0;
        b->r = 0;
        a->g = 0;
        b->g = 0;
        a->b = 0;
        b->b = 0;
        a->x = obj->pos_x + offsets[2 * i];
        b->x = obj->pos_x + offsets[2 * i];
        a->y = obj->pos_y + offsets[2 * i + 1];
        b->y = obj->pos_y + offsets[2 * i + 1];
        a->w = 0xc0;
        b->w = 0xc0;
        a->h = 0x10;
        b->h = 0x10;
        a++;
    }
}
