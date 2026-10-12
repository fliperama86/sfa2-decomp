/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u8 data_80028aa0_slot12[];
extern u16 *data_80028b40_slot12;

/* What it does (inferred, not an original name): fades a 16-entry palette
   one step toward a target palette. data_80028aa0_slot12 is the target (16
   halfwords of 5-5-5 colour), the table that data_80028b40_slot12 points at
   is the current palette. For each entry that differs, each of the three
   channels (red and green 5 bits, blue taken from bit 10 up, 6 bits, which
   the original does not mask either) becomes its current value plus 2, or
   the target's channel when that is not larger (so a channel above its
   target is set to the target); the entry is rewritten. The 16 entries are
   then loaded to the frame buffer rectangle (x 16, y 480, 16 wide, 1 high)
   and the function waits for the drawing to finish. It returns 1 when any
   entry differed, else 0. */
int fade_palette_toward_target_slot12(void) {
    Rect r;
    int i;
    unsigned green_stepped, red_stepped, blue, green, red, blue_target, green_target, red_target;
    u16 *target = (u16 *)data_80028aa0_slot12;
    int changed = 0;
    u16 *cur = data_80028b40_slot12;

    for (i = 0; i < 16; target++, i++, cur++) {
        if (*cur != *target) {
            changed = 1;
            blue_target = *target >> 10;
            /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
            /* Without it this function differs from the original in 13 instruction slots. */
            do {
                green_target = (*target >> 5) & 0x1f;
            } while (0);
            blue = (*cur >> 10) + 2;
            red_target = *target & 0x1f;
            green = (*cur >> 5) & 0x1f;
            red = *cur & 0x1f;
            if (blue >= blue_target) blue = blue_target;
            /* Stand-in, not recovered source: the statement below, which ORs to `changed` a term that is 0 for every value of it, reproduces the original's choice of registers. Without it this function differs from the original in 9 instruction slots. */
            /* The original has the blue target, the palette pointer and the green target in a3, t0 and t1; without the statement the build gives them t0, a3 and t1. The compiler hands out these registers by a figure made of each value's count of uses and the count of instructions it lives across, both taken before the pass that joins instructions. The statement is instructions at that time and none afterwards, so it lowers the figure of every value that lives across it (the pointer, the green and red targets) and not that of the blue target, whose last use stands above it. This reading is inferred from the compiler's dumps. */
            changed |= ((u32)changed << 24) & 0x100;
            green_stepped = green + 2;
            red_stepped = red + 2;
            if (green_stepped >= green_target) green_stepped = green_target;
            if (red_stepped >= red_target) red_stepped = red_target;
            *cur = red_stepped | ((blue << 10) | (green_stepped << 5));
        }
    }
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 1;
    func_80157fc4(&r, (u8 *)data_80028b40_slot12);
    func_80157d9c(0);
    return changed;
}
