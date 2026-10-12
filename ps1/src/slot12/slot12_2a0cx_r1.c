/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80022e48_slot12[];
extern Slot12Tile data_8002a394_slot12[];
extern Slot12Tile data_8002a794_slot12[];
void func_8015c150(void *prim);
void func_8015bfe8(void *prim, int flag);

/*
 * What it does (inferred, not an original name): sets up 14 pairs of
 * 16-byte rectangle records, 0x20 records apart, in one of two record
 * tables (the second when the object's table_index is not 0, else the first).
 * For each pair it calls the library on both records (one routine to
 * initialize, one to set a flag, called with 0), clears the colour bytes
 * and sets the position (the object's position plus an offset pair from a
 * table of 14 pairs of halfwords), width 0xc0 and height 0x10. Both
 * records of a pair get the same position.
 */
void func_80012a0c_slot12(Object *obj) {
    Slot12Tile *a;
    Slot12Tile *b;
    u16 *offsets = data_80022e48_slot12;
    int i;

    /* The second pointer is set in both arms; with b = a + 0x20 once after
       the if, this differs from the original in 19 instruction lines. */
    if (obj->table_index == 0) {
        a = data_8002a394_slot12;
        b = a + 0x20;
    } else {
        a = data_8002a794_slot12;
        b = a + 0x20;
    }
    for (i = 0; i < 14; i++) {
        func_8015c150(a);
        func_8015c150(b);
        func_8015bfe8(a, 0);
        func_8015bfe8(b, 0);
        a->r = 0;
        b->r = 0;
        a->g = 0;
        b->g = 0;
        a->b = 0;
        b->b = 0;
        a->x = obj->pos_x + *offsets;
        b->x = obj->pos_x + *offsets++;
        a->y = obj->pos_y + *offsets;
        b->y = obj->pos_y + *offsets++;
        a->w = 0xc0;
        b->w = 0xc0;
        a->h = 0x10;
        b->h = 0x10;
        a++;
        b++;
    }
}
