/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80119d08(AnimObj *a) {
    u16 *s0 = data_80183dc8[a->field_02];
    u16 *s1 = data_80183da0[a->field_02];
    u32 n = 2;
    do {
        func_8011a6b8(*s1++, *s0);
        n++;
        *s0++ = 0;
    } while (n < 9);
}

void func_80119d88(AnimObj *a) {
    func_80119e74(a, data_80183bb0[a->field_94] - data_80183ad0[a->field_94]);
}
