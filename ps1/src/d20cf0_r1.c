/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* A second name for the byte that externs.h declares as the scalar u8 data_801a8067 (four other units use the scalar; both names stand for the address 0x80198067). Read through the scalar, this function differs from the original in 7 instruction lines. */
extern u8 table_801a8067[];
/* Declared as an array here, not as the scalar u16 that 15 other units declare: read through the scalar this function differs from the original in 5 instruction lines. */
extern u16 data_801a6966[];
void func_80120f40(Object *object, u8 a1, u16 a2);
extern u8 table_8016e730[];

/* Inferred, not original names: starts a round. It counts a step in the status
   block data_8018f5a0, runs func_8011eb14, resets many fields of the game state,
   loads others from table_8016e664 and the bytes data_8016e685 to data_8016e688
   (field_13 gets 0xff when data_8016e688 is not 0) and from data_801a8067 and
   data_801a83fb, calls func_80120374 with field_10, sets field_110, head_a and
   head_b to -1 and the mode fields to fixed values, takes a random byte
   (get_random_byte masked with 0x78) into field_61, prepares both players with
   func_80120f40 and, for each bit of field_17 (1 left, 2 right), sets a few more
   fields of that player, then calls func_80138358 and clears three bytes. */
void func_80120cf0(GameState *state) {
    Object *l;
    Object *r;
    Tbl664 *t;
    int zero;
    int none;
    unsigned char rnd;
    data_8018f5a0->field_48++;
    func_8011eb14();
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 0 written at its uses this function differs from the original in 25 instruction lines. */
    zero = 0;
    state->field_04 = 0;
    state->field_05 = 0;
    state->mode = 0;
    state->field_1c = 0;
    state->frame_counter = 0;
    state->field_20 = 0;
    state->field_6d = 0;
    state->field_27 = 0;
    state->field_29 = 0;
    state->field_44 = 0;
    state->field_13c = 0;
    state->field_13d = 0;
    state->field_112 = 0;
    state->field_2c = 0;
    state->field_11e = 0;
    state->field_11f = 0;
    state->field_11b = table_801a8067[0];
    state->field_11c = data_801a83fb;
    l = &player_left;
    state->field_11 = data_8016e685;
    r = l + 1;
    t = (Tbl664 *)table_8016e664;
    /* The original loads this byte signed (lb), but externs.h declares it u8: with the plain test, and with (s8)data_8016e688 (the compiler narrows it back to lbu), this function differs from the original in 1 instruction line each. */
    if (*(s8 *)&data_8016e688 != 0) {
        state->field_13 = 0xff;
    } else {
        state->field_13 = data_8016e687;
    }
    state->field_15 = t->field_25 + 1;
    state->field_12 = t->field_26;
    state->field_14 = t->field_28;
    func_80120374(state->field_10);
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With -1 written at its uses this function differs from the original in 12 instruction lines. */
    none = -1;
    state->field_110 = none;
    state->field_113 = 1;
    state->field_09 = 1;
    state->field_14e = 6;
    state->field_150 = 6;
    state->field_07 = state->field_17;
    game_state.field_2b8 = 6;
    game_state.field_2ba = 6;
    state->head_a = none;
    state->head_b = none;
    state->field_2d = state->field_11;
    state->field_48 = state->field_15;
    state->field_5f = state->field_0e;
    /* The masked result goes through a byte local before its store: written in one statement, this function differs from the original in 2 instruction lines. */
    rnd = get_random_byte() & 0x78;
    state->field_61 = rnd;
    state->field_a4 = 7;
    func_80120f40(l, 0, data_801a6966[0]);
    func_80120f40(r, 1, data_801a6972);
    if (state->field_17 & 1) {
        l->field_a5 = 2;
        l->kind = zero;
        l->field_a9 = 3;
        l->field_cd = zero;
        l->field_f0 = zero;
    }
    if (state->field_17 & 2) {
        r->field_a5 = 2;
        r->kind = 1;
        r->field_a9 = 3;
        r->field_cd = zero;
        r->field_f0 = zero;
    }
    state->field_2e = zero;
    state->field_56 = table_8016e730[state->field_14];
    func_80138358(state);
    data_801a6938 = zero;
    data_801a6984[0] = 0;
    data_801a6985 = zero;
}
