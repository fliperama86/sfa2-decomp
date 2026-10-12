/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


s32 is_rising_a_near_in_front_of_b(Object *a, Object *b, Box32 *unused) {
    s16 x0;
    s16 x1;
    s16 d;
    int by;
    int side;

    if (data_80188f30 & 1) {
        if (a->active_state != 0 && a->state_2 != 8) {
            if (a->state_2 == 7) {
                return 0;
            }
            if (a->vel_y >= 0) {
                return 0;
            }
            x0 = a->pos_x;
            x1 = b->pos_x;
            side = x0 >= x1;
            if (b->facing_side != side) {
                return 0;
            }
            by = (u16)b->pos_y;
            if (a->pos_y < (s16)(by - 0x50)) {
                return 0;
            }
            d = x1 - x0;
            if (d < 0) {
                d = -d;
            }
            if (d < 0x50) {
                return 1;
            }
        }
    }
    return 0;
}
