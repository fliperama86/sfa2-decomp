/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_800285d4_slot28[];
extern u16 data_801a2be4[];
extern void (*data_80028a4c_slot28[])(Object *);

void func_80013b9c_slot28(void) {
    HudState *h = data_8018f5a0;
    h->field_4a = 9;
    game_state.field_c4 = 0;
    h->field_4c = 0;
    game_state.field_c6 = 0;
    h->field_4e = 0;
    game_state.field_c8 = 0;
    h->field_50 = 0;
    game_state.field_ca = 0;
    h->field_52 = 0;
    game_state.field_cc = 0;
    game_state.field_27 = 0;
    game_state.field_2c = 0;
    game_state.field_50 = 0;
    game_state.field_65 = 1;
    game_state.field_ab = 1;
    game_state.field_4f = 0;
    game_state.field_09 = 1;
    game_state.field_46 = 0;
    game_state.field_44 = 1;
}

void func_80013c38_slot28(Object *o) {
    u16 *a = data_801a2be4;
    u16 *b = a + 0xa00;
    int i;
    for (i = 0; i < 0x200; i++) {
        a[i] = data_800285d4_slot28[i];
        b[i] = data_800285d4_slot28[i];
    }
    data_80028a4c_slot28[((Slot28Obj *)o)->field_70 & 0x7f](o);
}
