/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

/* A second name for the byte that externs.h declares as the scalar u8 data_801a8067 (four other units use the scalar; both names stand for the address 0x80198067). Read through the scalar, this function differs from the original in 7 instruction lines. */
extern u8 table_801a8067[];
/* Declared as an array here, not as the scalar u16 that 15 other units declare: read through the scalar this function differs from the original in 5 instruction lines. */
extern u16 data_801a6966[];
void init_player_object(Object *object, u8 a1, u16 a2);
extern u8 table_8016e730[];

/* Inferred, not original names: starts a round. It counts a step in the status
   block data_8018f5a0, runs clear_players_arrays_state_and_init_pools, resets many fields of the game state,
   loads others from table_8016e664 and the bytes data_8016e685 to data_8016e688
   (table_value_b gets 0xff when data_8016e688 is not 0) and from data_801a8067 and
   data_801a83fb, calls set_sound_mono_or_stereo with setup_param, sets field_110, head_a and
   head_b to -1 and the mode fields to fixed values, takes a random byte
   (get_random_byte masked with 0x78) into random_value, prepares both players with
   init_player_object and, for each bit of side_bits (1 left, 2 right), sets a few more
   fields of that player, then calls init_counters_from_index_table and clears three bytes. */
void reset_game_state_for_round(GameState *state) {
    Object *l;
    Object *r;
    Tbl664 *t;
    int zero;
    int none;
    unsigned char rnd;
    data_8018f5a0->state_index++;
    clear_players_arrays_state_and_init_pools();
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With 0 written at its uses this function differs from the original in 25 instruction lines. */
    zero = 0;
    state->field_04 = 0;
    state->pause_flag = 0;
    state->mode = 0;
    state->field_1c = 0;
    state->frame_counter = 0;
    state->field_20 = 0;
    state->field_6d = 0;
    state->active_flag = 0;
    state->field_29 = 0;
    state->field_44 = 0;
    state->field_13c = 0;
    state->check_flag = 0;
    state->random_index = 0;
    state->state_2c_flag = 0;
    state->field_11e = 0;
    state->byte_table = 0;
    state->field_11b = table_801a8067[0];
    state->field_11c = data_801a83fb;
    l = &player_left;
    state->table_value_a = data_8016e685;
    r = l + 1;
    t = (Tbl664 *)table_8016e664;
    /* The original loads this byte signed (lb), but externs.h declares it u8: with the plain test, and with (s8)data_8016e688 (the compiler narrows it back to lbu), this function differs from the original in 1 instruction line each. */
    if (*(s8 *)&data_8016e688 != 0) {
        state->table_value_b = 0xff;
    } else {
        state->table_value_b = data_8016e687;
    }
    state->table_value_c = t->field_25 + 1;
    state->handler_index = t->field_26;
    state->table_index = t->field_28;
    set_sound_mono_or_stereo(state->setup_param);
    /* The local holds a constant: the original keeps this value in a register of its own. The name is inferred. */
    /* With -1 written at its uses this function differs from the original in 12 instruction lines. */
    none = -1;
    state->field_110 = none;
    state->field_113 = 1;
    state->field_09 = 1;
    state->field_14e = 6;
    state->field_150 = 6;
    state->side_mask = state->side_bits;
    game_state.left_rank_index = 6;
    game_state.right_rank_index = 6;
    state->head_a = none;
    state->head_b = none;
    state->table_index_2 = state->table_value_a;
    state->field_48 = state->table_value_c;
    state->mode_copy_2 = state->saved_value;
    /* The masked result goes through a byte local before its store: written in one statement, this function differs from the original in 2 instruction lines. */
    rnd = get_random_byte() & 0x78;
    state->random_value = rnd;
    state->reset_value = 7;
    init_player_object(l, 0, data_801a6966[0]);
    init_player_object(r, 1, data_801a6972);
    if (state->side_bits & 1) {
        l->field_a5 = 2;
        l->kind = zero;
        l->step_index = 3;
        l->mode_flag_2 = zero;
        l->field_f0 = zero;
    }
    if (state->side_bits & 2) {
        r->field_a5 = 2;
        r->kind = 1;
        r->step_index = 3;
        r->mode_flag_2 = zero;
        r->field_f0 = zero;
    }
    state->selection = zero;
    state->table_index_3 = table_8016e730[state->table_index];
    init_counters_from_index_table(state);
    data_801a6938 = zero;
    data_801a6984[0] = 0;
    data_801a6985 = zero;
}
