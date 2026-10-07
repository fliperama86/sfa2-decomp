/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190458;

#define SEL (*(int *)&game_state.field_354)

void func_801b44ec_slot04_sel(void) {
    ObjectRef *ref;
    int *sel = (int *)data_8019045c;
    int v;

    v = game_state.field_358->side;
    v ^= 1;
    *sel = v;
    if (((game_state.mode >> v) & 1) == 0) {
        goto zero;
    }
    {
        Object *p;

        ref = &data_80190458;
        p = &player_left;
        ref->p = p;
        if (v != 0) {
            ref->p = p + 1;
        }
    }
    v = game_state.field_358->kind;
    *sel = v;
    if (v == 0x11 || v == 0x13) {
        goto both;
    }
    if (data_80190458.p->kind == v) {
        goto same;
    }
    goto zero;
both:
    v = data_80190458.p->kind;
    SEL = v;
    if (v == 0x11) {
        goto same;
    }
    if (v != 0x13) {
        goto zero;
    }
same:
    {
        int *t = (int *)data_8019045c;

        v = game_state.field_358->field_d4;
        *t = v;
        if (data_80190458.p->field_d4 != v) {
            goto zero;
        }
        *t = 1;
        return;
    }
zero:
    SEL = 0;
}
