/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern void (*data_801b8ca8_slot04_sel[])(Object *, Slot04SelRec9d78 *);
extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];
extern u8 data_801ae028;

void func_801b4b18_slot04_sel(Object *unused_obj, Slot04SelRec9d78 *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *p = &player_left;
    Object *q = p + 1;
    Slot04SelRec9d78 *r = data_801b9d78_slot04_sel;
    Slot04SelRec9d78 *s = r + 1;
    game_state.field_6d = 0;
    data_801b8ca8_slot04_sel[1](p, r);
    data_801b8ca8_slot04_sel[1](q, s);
    if (game_state.field_05 != 0) {
        data_8018f5a0->field_4e = data_8018f5a0->field_4e + 1;
        data_8018f5a0->field_50 = 0;
        game_state.field_05 = 0;
    } else {
        data_8018f5a0->field_4e = data_8018f5a0->field_4e + 2;
        data_8018f5a0->field_50 = 0;
        game_state.field_80 = 0;
        game_state.field_b0 = 0;
        game_state.field_b1 = 0xff;
        game_state.field_b2 = 0;
    }
    data_801ae028 = 0;
}

void func_801b4c18_slot04_sel(Object *obj, Slot04SelRec9d78 *rec) {
    rec->field_00 = 0;
    rec->field_03 = 0;
    rec->field_04 = 0;
    rec->field_06 = 3;
    rec->field_07 = 0;
    rec->field_08 = 0;
    rec->field_09 = 0;
    rec->field_0a = 0;
    rec->field_0b = 0;
    rec->field_0c = 0;
    rec->field_0d = 0;
    rec->field_0e = 0;
    rec->field_0f = 0;
    rec->field_10 = 0;
    rec->field_11 = 0;
    rec->field_12 = 0;
    rec->field_13 = 0;
    rec->field_14 = 0;
    rec->field_15 = 0;
}
