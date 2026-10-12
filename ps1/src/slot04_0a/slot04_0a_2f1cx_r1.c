/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations, not original ones. */
u16 func_801b1e04_slot04_0a(Object *obj);
void func_801b30a0_slot04_0a(Object *obj);

/*
 * What it does (inferred, not an original name): a state of a character
 * object that starts a jump away from or towards the opponent. When
 * func_801b1e04_slot04_0a returns non-zero (low 16 bits) it calls
 * func_801b30a0_slot04_0a and returns. Otherwise, when the object's
 * vel_y is not above 0x50000 (signed) and one of the bits 0x80, 0x10, 4
 * or 1 (tested in that order) of its halfword at 0x134 is set, it selects
 * a number (0 for 0x80 and 1, 1 for 0x10, 2 for 4), adds one to state_3,
 * starts the sequence 0x2b plus that number with func_801307e0, sets
 * field_0b to 1 when the opponent's position x (object->other) is not less
 * than its own, else to 0, sets the velocity fields (when field_0b is 1:
 * vel_x -0x70000, accel_x 0x4000; when it is 0: vel_x 0x70000,
 * accel_x -0x4000; in both cases vel_y 0x38000 and accel_y -0x4000),
 * and calls play_side_sound_at, func_80141f28 and func_80138ae8. Then,
 * also when no bit was set or vel_y was too large, it calls
 * update_sequence.
 */
void func_801b2f1c_slot04_0a(Object *obj) {
    int a;
    int x;
    u8 b;
    u16 t;
    int vx;
    int vy;

    if (func_801b1e04_slot04_0a(obj)) {
        func_801b30a0_slot04_0a(obj);
        return;
    }
    a = obj->vel_y;
    /* Stand-in, not recovered source: the assignment of a to x, which nothing reads, reproduces the original's choice of registers. Without it this function differs from the original in 6 instruction slots. */
    x = a;
    /* Stand-in, not recovered source: the term that is ORed to x below, which is 0 for every value of a, reproduces the original's order of instructions. Without it this function differs from the original in 3 instruction slots. */
    x = a | ((int)(((u32)a << 21) & 2048));
    if (0x50000 < a) {
        goto end;
    }
    t = obj->pressed_bits;
    /* Stand-in, not recovered source: the term that is ORed to the first test below, which is 0 for every value of x, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
    if ((t & 0x80) | ((int)(((u32)x << 20) & 4096))) {
        x = 0;
    } else if (t & 0x10) {
        x = 1;
    } else if (t & 4) {
        x = 2;
    } else if (t & 1) {
        x = 0;
    } else {
        goto end;
    }
    obj->state_3++;
    func_801307e0(obj, (s16)(x + 0x2b));
    b = !(obj->other->pos_x < obj->pos_x);
    obj->field_0b = b;
    vx = 0x70000;
    vy = -0x4000;
    if (b) {
        vx = -0x70000;
        vy = 0x4000;
    }
    obj->vel_y = 0x38000;
    obj->accel_x = vy;
    obj->vel_x = vx;
    obj->accel_y = -0x4000;
    play_side_sound_at(obj, obj->side, 9);
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
end:
    update_sequence(obj);
}
