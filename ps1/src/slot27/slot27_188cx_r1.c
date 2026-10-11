/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/*
 * What it does (inferred, not an original name): copies a strip of 0x30
 * halfwords from a table in RAM into two rows of the table at
 * data_801a2824 (rows 0 and 5), then calls func_80137220 with two zero
 * arguments. The strip is chosen by the object's other object: its field_d4
 * and kind pick the source, its side picks where in the row the strip lands.
 */
void func_8001188c_slot27(Object *obj) {
    int i;
    int three;
    Object *other = obj->other;

    for (i = 0; i < 0x30; i++) {
        /* Stand-in, not recovered source: the assignment to the local `three`, which nothing reads, reproduces the original's order of instructions (the product kind * 3 is computed before the product field_d4 * 63). Without it this function differs from the original in 48 instruction slots. */
        data_801a2824[other->side * 0x30 + i] = *(u16 *)((u8 *)0x800e2000 + (((other->field_d4 * 0x3f) + (other->kind * (three = 3))) * 0x20 + i * 2));
        /* Stand-in, not recovered source: the assignment to the local `three`, which nothing reads, reproduces the original's order of instructions (the product kind * 3 is computed before the product field_d4 * 63). Without it this function differs from the original in 48 instruction slots. */
        data_801a2824[other->side * 0x30 + i + 0xa00] = *(u16 *)((u8 *)0x800e2000 + (((other->field_d4 * 0x3f) + (other->kind * (three = 3))) * 0x20 + i * 2));
    }
    func_80137220(0, 0);
}
