/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern Slot04SelRec9d38 data_801b9d38_slot04_sel[];
extern void (*data_801b7c44_slot04_sel[])(Object *, Slot04SelRec9d38 *);

void func_801b2224_slot04_sel(Object *unused_obj, Slot04SelRec9d38 *unused_rec) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    Object *left = &player_left;
    Object *right = left + 1;
    Slot04SelRec9d38 *rec = data_801b9d38_slot04_sel;
    Slot04SelRec9d38 *rec2 = rec + 1;

    game_state.field_6d = 0;
    data_801b7c44_slot04_sel[1](left, rec);
    data_801b7c44_slot04_sel[2](right, rec2);
    game_state.field_07 = 3;
    if (game_state.field_05 != 0) {
        data_8018f5a0->field_4e++;
        data_8018f5a0->field_50 = 0;
        game_state.field_05 = 0;
    } else {
        data_8018f5a0->field_4e += 2;
        data_8018f5a0->field_50 = 0;
        game_state.field_80 = 0;
        game_state.field_b0 = 0;
        game_state.field_b1 = 0xff;
        game_state.field_b2 = 0;
    }
}

void func_801b2318_slot04_sel(Object *obj, Slot04SelRec9d38 *rec) {
    rec->field_00 = 0;
    rec->field_03 = obj->kind;
    rec->field_04 = 0;
    rec->field_06 = data_8016e698[obj->side];
    rec->field_07 = 0;
    rec->field_08 = data_8016e698[obj->side + 2];
    rec->field_09 = 0;
    rec->field_0a = data_8016e698[4];
    rec->field_0b = 0;
    rec->field_0c = data_8016e698[5];
    rec->field_0d = 0;
    rec->field_0e = 0;
    rec->field_0f = 0;
    rec->field_10 = 0;
    rec->field_11 = 0;
    rec->field_12 = 0;
    rec->field_13 = 0;
    rec->field_14 = 0;
}
