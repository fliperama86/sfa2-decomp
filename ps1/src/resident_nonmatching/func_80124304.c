/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes (the build is 4 bytes
 * short) in how the pointer to the second pad record is advanced before
 * the second call to func_801246cc. The build does not use this file. The
 * differential test next to it (difftest.py, with func_80124304.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not original names): per-frame update of the
 * demo/replay recorder. It first runs func_801519b4 on four text buffers
 * and func_801246cc on each of the two players with their pad record
 * (data_80185fac, 3 bytes each). If game_state.field_17 is not 0 and one
 * of the two pad records has a non-zero third byte (c), it acts on the
 * mode byte data_8016e800:
 *   - 4, 5 or 6: clears both c bytes and does nothing else;
 *   - otherwise: increments field_52 of the HUD block data_8018f5a0,
 *     clears the input block data_801a6984 (field_00, field_01, index, last,
 *     flag), clears field_6a, field_19f, field_1a0, field_1a1 and field_1a2
 *     of both players, calls func_8013788c on each player, clears
 *     game_state.field_58 and field_6d, calls func_80124a7c with
 *     data_8016e801 and func_80124ae8 with data_8016e802, and copies
 *     data_8016e803 to the input block's field_05. Then by mode:
 *       1: calls func_801246a4(game_state, left, right); sets the input
 *          block's field_01 to 1; data_801ae028 = 0x88; saves the three
 *          bytes data_8016e801..803 in data_80185fbc, fc0, fc4; calls
 *          func_80124a7c and func_80124ae8 again with the first two;
 *          game_state.field_49 = 0x63; field_05 = data_8016e803 again; and
 *          zeroes the 4096 halfwords of the input block's buffer;
 *       2: calls func_801246a4(game_state, left, right); sets the input
 *          block's field_00 to 1; calls func_80124a7c with data_80185fbc
 *          and func_80124ae8 with data_80185fc0; game_state.field_49 =
 *          0x63; field_05 = data_80185fc4;
 *       3: clears both c bytes; sets the HUD block's field_4a to 1 and
 *          field_4c, field_4e, field_50 and field_52 to 0; game_state
 *          field_1a = 1, mode = 0, field_07 = field_17;
 *       any other value: nothing more.
 *
 * Contract:
 *   No arguments, no return value.
 *   Reads: game_state.field_17, the pad records data_80185fac (c bytes),
 *     data_8016e800 to data_8016e803, data_80185fbc, fc0, fc4, the HUD
 *     pointer data_8018f5a0 and its field_52.
 *   Writes: as listed above; the players are player_left and the next
 *     object after it (0x394 bytes later).
 *   Watched at every recorded call: game_state (0x200 bytes), both
 *     players, the input block, the HUD block and data_8018f5a0, the pad
 *     records, data_8016e800, data_80185fbc..fc4, data_801ae028.
 *   Callees replaced by recorders in both runs (each only logs its call
 *     and arguments, returns 0): func_801519b4 (1 argument),
 *     func_801246cc (2), func_801246a4 (3), func_8013788c (1),
 *     func_80124a7c (1, a byte), func_80124ae8 (1, a byte). What they do
 *     (they may change the pad records, the players and the game state) is
 *     outside this test, and so is the effect on this function of
 *     func_801246cc changing the c bytes before they are read.
 *   Aliasing: the HUD block is distinct from the game state and the
 *     players; the players, the input block and the pad records are fixed
 *     data of the tree.
 *   Excluded: none.
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Declared here because the published headers lack them (inferred types). */
extern TextBuf data_8016e92c;
extern TextBuf data_8016e95c;
extern TextBuf data_8016e98c;
extern Triple data_80185fac[];
extern u8 data_80185fbc;
extern u8 data_80185fc0;
extern u8 data_80185fc4;
extern u8 data_8016e801;
extern u8 data_8016e802;
extern u8 data_8016e803;
extern HudState *data_8018f5a0;
void func_801246cc(Object *a, Triple *b);
void func_801246a4(GameState *a, Object *b, Object *c);
void func_8013788c(Object *object);

void func_80124304(void) {
    int i;
    InputBlock *input;
    Object *left;
    Object *right;

    func_801519b4((Object *)&data_8016e8fc);
    func_801519b4((Object *)&data_8016e92c);
    func_801519b4((Object *)&data_8016e95c);
    func_801519b4((Object *)&data_8016e98c);
    left = &player_left;
    right = left + 1;
    func_801246cc(left, &data_80185fac[0]);
    func_801246cc(right, &data_80185fac[1]);
    input = (InputBlock *)data_801a6984;
    if (game_state.field_17 == 0) return;
    if (data_80185fac[0].c == 0 && data_80185fac[1].c == 0) return;

    if (data_8016e800 == 4 || data_8016e800 == 5 || data_8016e800 == 6) {
        data_80185fac[0].c = 0;
        data_80185fac[1].c = 0;
        return;
    }
    data_8018f5a0->field_52++;
    input->index = 0;
    input->last = 0;
    input->flag = 0;
    input->field_00 = 0;
    input->field_01 = 0;
    left->field_6a = 0;
    left->field_19f = 0;
    left->field_1a0 = 0;
    left->field_1a1 = 0;
    left->field_1a2 = 0;
    right->field_6a = 0;
    right->field_19f = 0;
    right->field_1a0 = 0;
    right->field_1a1 = 0;
    right->field_1a2 = 0;
    func_8013788c(left);
    func_8013788c(right);
    game_state.field_58 = 0;
    game_state.field_6d = 0;
    func_80124a7c(data_8016e801);
    func_80124ae8(data_8016e802);
    input->field_05 = data_8016e803;
    if (data_8016e800 == 1) {
        func_801246a4(&game_state, left, right);
        input->field_01 = 1;
        data_801ae028 = 0x88;
        data_80185fbc = data_8016e801;
        data_80185fc0 = data_8016e802;
        data_80185fc4 = data_8016e803;
        func_80124a7c(data_8016e801);
        func_80124ae8(data_8016e802);
        game_state.field_49 = 0x63;
        input->field_05 = data_8016e803;
        for (i = 0; i < 0x1000; i++) {
            input->buf[i] = 0;
        }
    } else if (data_8016e800 == 2) {
        func_801246a4(&game_state, left, right);
        input->field_00 = 1;
        func_80124a7c(data_80185fbc);
        func_80124ae8(data_80185fc0);
        game_state.field_49 = 0x63;
        input->field_05 = data_80185fc4;
    } else if (data_8016e800 == 3) {
        data_80185fac[0].c = 0;
        data_80185fac[1].c = 0;
        data_8018f5a0->field_4a = 1;
        data_8018f5a0->field_4c = 0;
        data_8018f5a0->field_4e = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        game_state.field_1a = 1;
        game_state.mode = 0;
        game_state.field_07 = game_state.field_17;
    }
}
