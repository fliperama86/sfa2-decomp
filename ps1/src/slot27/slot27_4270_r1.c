/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HalfPair data_80026ef0_slot27[];

void func_80014270_slot27(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void noop_slot00_slot27(Object *obj) {
}

void copy_other_palette_to_rows_slot27(Object *obj) {
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
    queue_image_upload_job(0, 0);
}

void set_position_from_side_table_of_other_slot27(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    int side = obj->other->side;
    obj->pos_x = data_80026ef0_slot27[side].pos_x;
    obj->pos_y = data_80026ef0_slot27[side].pos_y;
}
