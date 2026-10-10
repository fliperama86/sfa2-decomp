/*
 * Nonmatching. This function is NOT byte-identical to the original: after
 * the call to func_80151184 the original masks the result with 0x78 before
 * it moves the argument of the next call; the code built from this C moves
 * the argument first (two instruction slots). The exact owner of the bytes
 * in the PS1 build stays the raw bytes of the resident image; the build
 * does not use this file. The differential test next to it (difftest.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): starts a round. It counts
 * a step in the status block data_8018f5a0, runs func_8011eb14, resets
 * many fields of the game state, loads others from the tables at
 * table_8016e664 and data_8016e685 to data_8016e688 (field_13 gets 0xff
 * when data_8016e688 is not 0), calls func_80120374 with field_10, sets
 * head_a and head_b to -1 and the mode fields to fixed values, draws a
 * random byte (func_80151184 masked with 0x78) into field_61, prepares
 * both players with func_80120f40 and, for each bit of field_17 (1 left,
 * 2 right), sets a few more fields of that player, then calls
 * func_80138358 and clears three bytes.
 *
 * Contract:
 *   Argument: the original takes the address of the game state in its
 *     first argument register (a0) and never names the global game_state.
 *     The tree declares the function with that argument (protos.h, GameState
 *     *state), and its one caller (func_80120ca0) leaves the address in a0.
 *     This C does not read the parameter; it
 *     uses the global game_state, which is the same memory in every call
 *     the game makes. The contract passes &game_state in a0; a call with
 *     another pointer is outside the contract, because the C does not
 *     read a0 and would act on game_state instead. No return value.
 *   Reads: data_8018f5a0 (a pointer to a block, field_48 is read and
 *     written), the bytes data_801a8067 and data_801a83fb, the halfwords
 *     data_801a6966 and data_801a6972, data_8016e685, data_8016e687,
 *     data_8016e688, table_8016e664 at offsets 0x25, 0x26 and 0x28,
 *     table_8016e730 indexed by game_state.field_14, game_state.field_0e,
 *     field_10, field_17 and the fields it has written.
 *   Reads and writes through the callees that run as original code (the C
 *     does not name them): the seed word data_80190126 (func_80151184
 *     reads and rewrites it), table_6cf0 (func_80138358 reads the entry
 *     game_state.field_2d), and counter_a, counter_b and counter_c
 *     (func_80138358 writes them), game_state.field_6e.
 *   Writes: about thirty game_state fields (the list is the function), the
 *     two players player_left and player_right (the fields set by
 *     func_80120f40 and, by bit of field_17, field_a5, kind, field_a9,
 *     field_cd and field_f0), data_801a6938, data_801a6984,
 *     data_801a6985, and what the running callees write.
 *   Watched at every call (copied into the log): game_state, both players,
 *     the status block of data_8018f5a0, and the words at data_801a6938
 *     and data_801a6984.
 *   Callees replaced by recorders, the same in both runs: func_8011eb14
 *     (takes no argument, returns 0; it runs the whole round
 *     initialisation chain) and func_80120374 (takes one argument; it
 *     reaches Sony's library). func_80151184 (a generator of 8-bit values
 *     from a seed word in RAM), func_80120f40 and func_80138358 run as the
 *     original code in both runs.
 *   Aliasing: the status block, the players and game_state are distinct.
 *   Not reached by any input: none expected; see the coverage line.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; not original ones. */
extern u8 table_8016e730[];
extern u16 data_801a6966;
void func_80120f40(Object *player, int side, u16 value);

void func_80120cf0(GameState *state) {
    GameState *g = &game_state;
    Tbl664 *t = (Tbl664 *)table_8016e664;
    Object *l = &player_left;
    Object *r = &player_right;

    data_8018f5a0->field_48++;
    func_8011eb14();
    g->field_04 = 0;
    g->field_05 = 0;
    g->mode = 0;
    g->field_1c = 0;
    g->field_1d = 0;
    g->field_20 = 0;
    g->field_6d = 0;
    g->field_27 = 0;
    g->field_29 = 0;
    g->field_44 = 0;
    g->field_13c = 0;
    g->field_13d = 0;
    g->field_112 = 0;
    g->field_2c = 0;
    g->field_11e = 0;
    g->field_11f = 0;
    g->field_11b = data_801a8067;
    g->field_11c = data_801a83fb;
    g->field_11 = data_8016e685;
    if (data_8016e688 != 0) {
        g->field_13 = 0xff;
    } else {
        g->field_13 = data_8016e687;
    }
    g->field_15 = t->field_25 + 1;
    g->field_12 = t->field_26;
    g->field_14 = t->field_28;
    func_80120374(g->field_10);
    g->field_110 = -1;
    g->field_113 = 1;
    g->field_09 = 1;
    g->field_14e = 6;
    g->field_150 = 6;
    g->field_07 = g->field_17;
    game_state.field_2b8 = 6;
    game_state.field_2ba = 6;
    g->head_a = -1;
    g->head_b = -1;
    g->field_2d = g->field_11;
    g->field_48 = g->field_15;
    g->field_5f = g->field_0e;
    g->field_61 = func_80151184() & 0x78;
    g->field_a4 = 7;
    func_80120f40(l, 0, data_801a6966);
    func_80120f40(r, 1, data_801a6972);
    if (g->field_17 & 1) {
        l->field_a5 = 2;
        l->kind = 0;
        l->field_a9 = 3;
        l->field_cd = 0;
        l->field_f0 = 0;
    }
    if (g->field_17 & 2) {
        r->field_a5 = 2;
        r->kind = 1;
        r->field_a9 = 3;
        r->field_cd = 0;
        r->field_f0 = 0;
    }
    g->field_2e = 0;
    g->field_56 = table_8016e730[g->field_14];
    func_80138358(g);
    data_801a6938 = 0;
    data_801a6984[0] = 0;
    data_801a6985 = 0;
}
