/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801b6d60_slot04_sel[])(Object *, Slot04SelRec *);
extern Slot04SelRec data_801b9cf0_slot04_sel[];

void func_801b0090_slot04_sel(Object *unused_obj, Slot04SelRec *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *l = &player_left;
    Object *r = l + 1;
    Slot04SelRec *a = data_801b9cf0_slot04_sel;
    Slot04SelRec *b = a + 1;

    game_state.field_6d = 0;
    data_801b6d60_slot04_sel[1](l, a);
    data_801b6d60_slot04_sel[2](r, b);
    if (game_state.field_05 != 0) {
        data_8018f5a0->field_4e += 1;
        data_8018f5a0->field_50 = 0;
        game_state.field_05 = 0;
    } else {
        data_8018f5a0->field_4e += 2;
        data_8018f5a0->field_50 = 0;
        game_state.field_80 = 0;
        game_state.field_b0 = 0;
        game_state.field_b1 = 0xff;
        game_state.field_b2 = 0;
        game_state.field_09 = 0;
    }
}

void func_801b0184_slot04_sel(Object *obj, Slot04SelRec *rec) {
    rec->field_00 = 0;
    rec->field_03 = obj->kind;
    rec->field_04 = 0;
    rec->field_08 = data_8016e694[obj->side];
    rec->field_09 = 0;
    rec->field_0a = data_8016e696;
    rec->field_0b = 0;
    rec->field_12 = 0;
    rec->field_13 = 0;
    rec->field_14 = 0;
    rec->field_15 = 0;
    rec->field_16 = 0;
    rec->field_17 = 0;
    rec->field_18 = 0;
}
