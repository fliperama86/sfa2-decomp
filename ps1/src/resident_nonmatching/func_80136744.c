/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs in register allocation (two pointers to the players
 * swap registers, which shifts later temporaries by one register). The exact
 * owner of the bytes in the PS1 build stays the raw bytes of the original;
 * the build does not use this file. The differential test next to it
 * (func_80136744.py, run with difftest.py) compares this C with the
 * original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): one step of a vertical
 * scroll follower. It picks a target height from the two players' pos_y: if
 * player_left has bit 7 of field_73 set and field_261 equal to 0, the
 * target is 0xd0 - player_right.pos_y; else if player_right has that state,
 * it is 0xd0 - player_left.pos_y; else it is the larger (as signed 16-bit)
 * of 0xd0 - pos_y of the two. The sprite's field_26 is then moved one unit
 * toward target - 0x39 (16-bit arithmetic): up by 1 when that is above it,
 * down by 1 when below it, and left alone when equal; the stored field_26
 * is not capped. The value passed on is, after a move up, the new field_26
 * or field_40 if that is smaller; after a move down, the new field_26 or
 * field_42 if that is larger (signed 16-bit comparisons); when equal, the
 * unchanged field_26. func_801368ac(sprite, value) is called last.
 *
 * Contract:
 *   Argument: a0 = pointer to a sprite (the prototype's type). No result.
 *   Reads: player_left and player_right (field_73, field_261, pos_y); the
 *     sprite's field_26, field_40, field_42.
 *   Writes: the sprite's field_26 (not when the follower is on target) and
 *     whatever func_801368ac writes.
 *   Callees: func_801368ac runs as the original code in both runs (it is
 *     four instructions: it reads the sprite's halfword at 0x3a and writes
 *     the sprite's field_26; read from the original's listing, not
 *     tested); no recorder.
 *   Aliasing: the sprite and the two players are distinct blocks.
 *   Excluded inputs: none. Every instruction slot is executed.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801368ac(Cam *cam, int delta);

void func_80136744(Sprite *sprite) {
    s16 target;
    s16 other;
    s16 delta;
    s16 limit;
    u16 next;

    if ((player_left.field_73 & 0x80) && player_left.field_261 == 0) {
        target = 0xd0 - player_right.pos_y;
    } else if ((player_right.field_73 & 0x80) && player_right.field_261 == 0) {
        target = 0xd0 - player_left.pos_y;
    } else {
        other = 0xd0 - player_left.pos_y;
        target = 0xd0 - player_right.pos_y;
        if (target < other) target = other;
    }
    delta = target - sprite->field_26 - 0x39;
    limit = sprite->field_26;
    if (delta > 0) {
        next = sprite->field_26 + 1;
        sprite->field_26 = next;
        limit = sprite->field_40;
        if ((s16)next < limit) limit = next;
    } else if (delta < 0) {
        next = sprite->field_26 - 1;
        sprite->field_26 = next;
        limit = sprite->field_42;
        if (limit < (s16)next) limit = next;
    }
    func_801368ac((Cam *)sprite, limit);
}
