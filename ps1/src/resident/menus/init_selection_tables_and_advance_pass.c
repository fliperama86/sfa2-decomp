/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/*
 * What it does (inferred, not original names): prepares the tables of a
 * two-column selection screen. It advances a counter of the HUD state
 * (pass_index of data_8018f5a0) when game_state.versus_flag is not 0, or calls
 * add_item_to_table with one of two blocks (chosen by game_state.field_31) and,
 * if the player selected by field_31 has bit 0x100 in input_bits, advances the
 * counter and calls play_sound_at. Then it clears the cursor bytes, sets
 * data_8018d264 to 1, copies two rows of 8 ids into the 5-byte entries of two
 * tables and gives each entry a kind byte (0x1a for the ids 0x94 to 0x97,
 * else 0x1d), and marks the cursor entries of the other tables (0x14 current,
 * 0x1a and 0x1b others).
 */
void init_selection_tables_and_advance_pass(void) {
    GameState *g = &game_state;
    u8 i;
    u8 id;

    /* field_31 is read again after add_item_to_table, which may change it
       (inferred; the differential test of the readable version covers it). */
    if (g->versus_flag == 0) {
        if (g->field_31 == 1) {
            add_item_to_table(data_80181044);
        } else {
            add_item_to_table(data_80181060);
        }
        if ((g->field_31 == 1 && (player_left.input_bits & 0x100)) || (g->field_31 == 2 && (player_right.input_bits & 0x100))) {
            data_8018f5a0->pass_index++;
            play_sound_at(0, 0, 0x34c);
        }
    } else {
        data_8018f5a0->pass_index++;
    }
    i = 0;
    data_8018d250 = 0;
    data_8018d254 = 0;
    data_8018d258 = 0;
    data_8018d25c = 0;
    data_8018d260 = 0;
    data_8018d264 = 1;
    do {
        data_8018125e[i * 5 + 1] = table_8016e664[2][i];
        data_8018128a[i * 5 + 1] = table_8016e664[3][i];
        i++;
    } while (i < 8);
    i = 0;
    do {
        id = data_8018125e[i * 5 + 1];
        if (id == 0x94 || id == 0x95 || id == 0x96 || id == 0x97) {
            data_8018125e[i * 5] = 0x1a;
        } else {
            data_8018125e[i * 5] = 0x1d;
        }
        id = data_8018128a[i * 5 + 1];
        if (id == 0x94 || id == 0x95 || id == 0x96 || id == 0x97) {
            data_8018128a[i * 5] = 0x1a;
        } else {
            data_8018128a[i * 5] = 0x1d;
        }
        i++;
    } while (i < 8);
    data_80181094[data_8018d250 * 14 + 1] = 0x1a;
    data_80181138[data_8018d254 * 14 + 1] = 0x1a;
    i = 1;
    data_801811aa[data_8018d260 * 16] = 0x14;
    table_801811ed[data_8018d264 * 11] = 0x14;
    data_80181206[data_8018d260 * 16] = 0x14;
    data_8018124a[data_8018d260 * 9] = 0x14;
    do {
        data_80181094[i * 14 + 1] = 0x1b;
        data_80181138[i * 14 + 1] = 0x1b;
        i++;
    } while (i < 8);
    data_801811aa[16] = 0x1a;
    data_801811aa[32] = 0x1a;
    table_801811ed[0] = 0x1a;
    data_80181206[16] = 0x1a;
    data_80181206[32] = 0x1a;
    data_80181206[48] = 0x1a;
    data_8018124a[9] = 0x1a;
}
