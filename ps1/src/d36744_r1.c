/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

void func_801368ac(Cam *cam, int delta);

/* What it does (inferred, not an original name): one step of a vertical
   scroll follower. It picks a target height from the two players' pos_y: the
   other player's 0xd0 - pos_y when one has bit 7 of field_73 set and field_261
   equal to 0, else the larger (as signed 16-bit) of the two. The sprite's
   field_26 moves one unit toward target - 0x39 (16-bit arithmetic). The value
   given to func_801368ac is the new field_26 clamped by field_40 after a move
   up, by field_42 after a move down, and the unchanged field_26 when equal.
   field_26 is stored uncapped. */
void func_80136744(Sprite *sprite) {
    Object *left = &player_left;
    /* The second player is reached from the first: with &player_right 14 instruction lines differ. */
    Object *right = left + 1;
    s16 target;
    s16 field;
    s16 delta;
    s16 next;
    s16 value;

    if (left->field_73 & 0x80 && left->field_261 == 0) {
        target = 0xd0 - right->pos_y;
    } else if (right->field_73 & 0x80 && right->field_261 == 0) {
        target = 0xd0 - left->pos_y;
    } else {
        value = 0xd0 - left->pos_y;
        target = 0xd0 - right->pos_y;
        if (value > target) {
            target = value;
        }
    }
    field = sprite->field_26;
    delta = target - field - 0x39;
    if (delta > 0) {
        next = field + 1;
        sprite->field_26 = next;
        /* value held the left player's height above and is used again here for the limit; a separate local for the limit differs in 24 instruction lines. */
        value = sprite->field_40;
        if (next < value) {
            value = next;
        }
        /* One call after the three arms (value = field in the last) differs in 8 instruction lines. */
        func_801368ac((Cam *)sprite, value);
    } else if (delta < 0) {
        next = field - 1;
        sprite->field_26 = next;
        value = sprite->field_42;
        if (value < next) {
            value = next;
        }
        func_801368ac((Cam *)sprite, value);
    } else {
        func_801368ac((Cam *)sprite, field);
    }
}
