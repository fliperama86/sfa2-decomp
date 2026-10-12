/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* Callees of the follower; not in the shared headers. Inferred. */
void func_80136668(Cam *cam, short step);
void func_801366d4(Cam *cam, short step);

/*
 * One step of a horizontal scroll follower (inferred). It takes the span
 * [lo, hi] that the players cover (each covers pos_x +- half_width), then
 * moves the sprite with func_80136668 or func_801366d4 toward the span's
 * middle when the span is 0xc0 or wider, else toward its edges, or calls
 * set_cam_pos_x when neither edge is out of range.
 */
void func_801364a0(Sprite *sprite) {
    Object *left = &player_left;
    /* The second player is reached from the first: with &player_right 29 instruction lines differ. */
    Object *right = left + 1;
    u16 lo, hi, other, other2;
    s16 amount;

    /* player_left has bit 7 of status_flag set and field_261 equal to 0: the span is player_right's; the next arm is the mirror case. */
    if ((left->status_flag & 0x80) && left->field_261 == 0) {
        lo = right->pos_x;
        hi = lo;
        lo -= right->half_width;
        hi += right->half_width;
    } else if ((right->status_flag & 0x80) && right->field_261 == 0) {
        lo = left->pos_x;
        hi = lo;
        lo -= left->half_width;
        hi += left->half_width;
    } else {
        /* Judgment call: the union is taken as signed 16-bit values, so the compares cast to s16. */
        lo = left->pos_x;
        other = right->pos_x;
        lo -= left->half_width;
        other -= right->half_width;
        if ((s16)other < (s16)lo) {
            lo = other;
        }
        hi = left->pos_x;
        other2 = right->pos_x;
        hi += left->half_width;
        other2 += right->half_width;
        if ((s16)other2 > (s16)hi) {
            hi = other2;
        }
    }
    /*
     * Judgment call: the width and the middle are 16-bit signed values. The
     * pairs "amount = hi; amount -= lo;" (here and below) are what the bytes need;
     * written as "hi - lo", "(hi + lo)" and "hi - field_22 - 0x120" the three differ in 15
     * instruction lines (alone: 10, 11 and 1).
     */
    amount = hi;
    amount -= lo;
    if (amount >= 0xc0) {
        amount = hi;
        amount += lo;
        amount = (amount >> 1) - sprite->field_22 - 0xc0;
        if (amount < 0) {
            func_801366d4((Cam *)sprite, amount);
        } else {
            func_80136668((Cam *)sprite, amount);
        }
    } else {
        amount = hi;
        amount -= sprite->field_22;
        amount = amount - 0x120;
        if (amount >= 0) {
            func_80136668((Cam *)sprite, amount);
        } else {
            amount = lo - sprite->field_22 - 0x60;
            if (amount <= 0) {
                func_801366d4((Cam *)sprite, amount);
            } else {
                /* The tree declares set_cam_pos_x with a Cam pointer; the sprite is cast to it. */
                set_cam_pos_x((Cam *)sprite, (s16)sprite->field_22);
            }
        }
    }
}
