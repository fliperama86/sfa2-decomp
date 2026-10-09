/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HalfPair data_80026ef0_slot27[];
void func_8011f240(Slab172 *s);

void func_80014270_slot27(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_80014290_slot27(Object *obj) {
}

void func_80014298_slot27(Object *obj) {
    int i;
    Object *other = obj->other;
    for (i = 1; i < 16; i++) {
        u16 *src = (u16 *)((i * 2 + (other->kind * 3 * 64 + other->field_d4 * 32)) + (u8 *)0x800e1000);
        int c = *src;
        if (c == 0) {
            c = 0x421;
        }
        data_801a27e4_rows[0][other->side * 16 + i] = c;
        data_801a27e4_rows[5][other->side * 16 + i] = c;
    }
    func_80137220(0, 0);
}

void func_8001435c_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int side = obj->other->side;
    obj->pos_x = data_80026ef0_slot27[side].field_00;
    obj->pos_y = data_80026ef0_slot27[side].field_02;
}
