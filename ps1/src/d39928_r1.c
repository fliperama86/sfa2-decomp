/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/*
 * Reconstruction of a box test (inferred, not an original name): tests whether
 * the box of object b, placed at b's position, is apart from the cursor box,
 * placed at a's position. Boxes are four halfwords: x, y, width, height. An
 * object whose field_0b is not 0 has its box mirrored in x. Returns 1 when
 * apart in x, else the same test on y; stores the placements it used.
 */
u8 func_80139928(Object *a, Object *b, Box32 *box) {
    Quad16 *cursor;
    u16 box_x;
    u16 box_y;
    u16 box_w;
    u16 box_h;
    u16 cur_x;
    u16 cur_y;
    u16 cur_w;
    u16 cur_h;
    u16 delta;
    s16 distance;
    box_x = box->origin;
    cursor = (Quad16 *)game_state.cursor;
    cur_x = cursor->v[0];
    box_y = box->field_02;
    cur_w = cursor->v[2];
    box_w = box->endpoint;
    cur_y = cursor->v[1];
    cur_h = cursor->v[3];
    box_h = box->field_06;
    if (a->field_0b != 0) {
        cur_x = -cur_x;
    }
    if (b->field_0b != 0) {
        box_x = -box_x;
    }
    /* The placements go into the variables that held the boxes' values. */
    cur_x = cur_x + a->pos_x;
    box_x = box_x + b->pos_x;
    data_80188ed0.out_3c = cur_x;
    data_80188ed0.out_40 = box_x;
    delta = box_x - cur_x;
    distance = delta;
    if ((s16)delta < 0) {
        distance = -delta;
    }
    /* The result of the comparison goes into a variable before it is tested: tested directly, this
       function differs from the original in 6 instruction lines. */
    box_x = (s16)(box_w + cur_w) < distance;
    if (box_x) {
        return 1;
    }
    /* The same for y: with locals of their own for the two y placements this function differs from
       the original in 5 instruction lines. */
    cur_y = a->pos_y - cur_y;
    box_y = b->pos_y - box_y;
    data_80188ed0.out_44 = cur_y;
    data_80188ed0.out_48 = box_y;
    delta = box_y - cur_y;
    box_y = delta;
    if ((s16)delta < 0) {
        box_y = -delta;
    }
    return (s16)(box_h + cur_h) < (s16)box_y;
}
