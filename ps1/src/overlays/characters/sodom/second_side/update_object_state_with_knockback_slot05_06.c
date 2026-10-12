/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

void update_gauge_from_pressed_bits(Object *object);
int spawn_table_effect_at_object_index_two(Object *object);
void move_x_by_vel_x_facing_then_accelerate_slot05_06(Object *obj);
void negate_value_if_facing_side_zero_slot05_06(Object *obj, u16 *p);
int spawn_object_copying_position_slot05_06(Object *obj);
extern u16 data_801dd478_slot05_06[];

/*
 * What it does (inferred, not an original name): a per-frame update of a
 * character state chosen by step_flags. State 2 sets facing_side from pos_x;
 * state 0 does nothing; otherwise, with vel_x >= 0, it runs the move,
 * may knock back ref_other.p, then update_sequence. With vel_x negative
 * it counts table_value down and either ends the state (state_3 + 1,
 * sequence 0x2e) or starts the sequence from data_801dd478_slot05_06.
 * The twin of update_multi_hit_sequence_with_knockback_slot04_06 on the second side (same code, a callee that returns an int).
 * Casts to Slot04aObj: table_value is a member of the module's record type
 * only; a local pointer of that type instead of the casts differs from
 * the original in 49 instruction lines.
 */
void update_object_state_with_knockback_slot05_06(Object *obj) {
    GameState *state = &game_state;
    /* Stand-in, not recovered source: a second name for the address of game_state (the original keeps it in two registers, s1 and a copy in s2, and reads the two tests of frame_counter through the copy) reproduces the original's choice of registers. Without it this function differs from the original in 15 instruction slots. */
    GameState *state_late = state;
    u8 kind = obj->step_flags;
    u8 hit;
    u16 d;
    if (kind == 2) {
        if (obj->pos_x > 0x200) {
            if (obj->pos_x < 0x301) {
                update_sequence(obj);
                return;
            }
        }
        obj->facing_side = 0;
        if (obj->pos_x < 0x280) {
            obj->facing_side = 1;
        }
        update_sequence(obj);
        return;
    }
    if (kind == 0) {
        update_sequence(obj);
        return;
    }
    /* Stand-in, not recovered source: the test of kind against 2 written a second time, which is never true here, reproduces the original's order of instructions. Without it this function differs from the original in 19 instruction slots. */
    /* The tested value is kind, already known not to be 2 (the first test returned): the original still has `bne v0,v1,...` with v1 still 2, then `li v0,0x18`, the store of 0x18 to state_param and `jal spawn_two_effects_at_other_ground`. The bytes do not say which condition the source had. */
    if (kind == 2) {
        game_state.state_param = 0x18;
        spawn_two_effects_at_other_ground(obj);
    }
    if (obj->vel_x >= 0) {
        move_x_by_vel_x_facing_then_accelerate_slot05_06(obj);
        if (obj->mode_flag_2 != 0) {
            update_gauge_from_pressed_bits(obj);
            /* The result is masked to a byte; without the cast this function differs from the original in 25 instruction lines. */
            if ((u8)drain_gauge_by_other_input(obj) != 0) {
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, 0x2e);
                return;
            }
        }
        if ((state_late->frame_counter & 1) == 0) {
            /* The unsigned cast changes no value (a byte plus 3); without it this function differs from the original in 1 instruction line. */
            if (((Slot04aObj *)obj)->table_value < (u32)(obj->direction_index >> 1) + 3 || (((Slot04aObj *)obj)->table_value & 0x80) != 0) {
                if (spawn_object_copying_position_slot05_06(obj) != 0) {
                    d = (get_random_byte() & 0xf) + 0x5c;
                    negate_value_if_facing_side_zero_slot05_06(obj, &d);
                    ref_other.p->pos_x += d;
                    d = get_random_byte();
                    d &= 0xf;
                    ref_other.p->pos_y -= d;
                }
            }
        }
        if ((state_late->frame_counter & 3) == 0) {
            /* The knock-back block (b2) and the final call are shared by three paths through gotos, as in the original; written with nested ifs and a flag this function differs from the original in 30 instruction lines. */
            if (((Slot04aObj *)obj)->table_value == 1) {
                /* The result is read as a signed byte; without the cast this function differs from the original in 20 instruction lines. */
                if ((s8)spawn_table_effect_at_object_index_two(obj) != 0) {
                    goto b2;
                }
                goto call;
            }
            d = get_random_byte();
            if ((d &= 0xf) != 0) {
                goto alt;
            }
            hit = spawn_random_offset_object_variant_zero(obj);
test:
            if (hit != 0) {
                goto b2;
            }
            goto call;
alt:
            hit = spawn_random_offset_object_variant_one(obj);
            goto test;
b2:
            d = 0x60;
            negate_value_if_facing_side_zero_slot05_06(obj, &d);
            ref_other.p->pos_x += d;
            d = get_random_byte();
            d &= 0xf;
            ref_other.p->pos_y -= d;
        }
call:
        update_sequence(obj);
    } else {
        ((Slot04aObj *)obj)->table_value--;
        if (((Slot04aObj *)obj)->table_value != 0) {
            apply_damage_to_other_return_defeated(obj, -0xfe, 0);
            play_side_sound_at(obj, obj->side, 8);
            /* Without the signed cast the compiler warns and this function differs from the original in 45 instruction lines. */
            if ((s16)obj->other->health >= 0) {
                play_sound_at(obj->other, obj->other->side, 0x34e);
            } else {
                play_sound_at(obj->other, obj->other->side, 0x34f);
            }
            obj->vel_x = 0xa0000;
            obj->accel_x = -0xe000;
            state->state_param = 8;
            d = data_801dd478_slot05_06[((Slot04aObj *)obj)->table_value];
            set_sequence_by_side_and_mode(obj, d);
        } else {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x2e);
        }
    }
}
