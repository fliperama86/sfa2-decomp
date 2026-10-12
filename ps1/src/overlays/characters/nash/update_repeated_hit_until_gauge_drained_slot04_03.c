/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* What it does (inferred, not an original name): a state of a character that is struck or held by the other object. It stores the other object (field_40) in ref_other. When the low byte of step_flags is not 0 it clears that byte, makes a sound, calls spawn_effect_at_object_offset and apply_damage_to_other_return_defeated (with 8 the first time field_1cf is 0, which then becomes 0xff, otherwise 0). If apply_damage_to_other_return_defeated returns a nonzero byte it calls set_other_hit_reaction_state_0, sounds 0x340 for the other object and goes to the advance. Otherwise it adds 1 to countdown and sounds 0x30b. Then (and when the low byte of step_flags was 0) it calls update_sequence_by_pressed_bytes; if countdown is 0 it hands over to update_sequence; otherwise it reloads ref_other from field_40 and advances when the other object's field_15b is 0 or drain_gauge_by_other_input returns a nonzero byte, else hands over to update_sequence. The advance adds 1 to state_3, sounds 0x30d for the other object and starts sequence 0x41. */
void update_repeated_hit_until_gauge_drained_slot04_03(Object *obj) {
    /* Stand-in, not recovered source: the local r, which holds the address of ref_other and is set again in three places (the original keeps it in a register of its own), reproduces the original's choice of registers. Without it this function differs from the original in 53 instruction slots. */
    ObjectRef *r;
    /* k is a short: as an int this function differs from the original in 6 instruction slots (the original widens the argument of apply_damage_to_other_return_defeated with sll/srl 16). */
    s16 k;
    /* The do-while wrapper reproduces the original's choice of registers: the compiler counts a reference inside it twice. It adds no instruction. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
    do {
        r = &ref_other;
        r->p = obj->other;
    } while (0);
    if ((obj->step_flags & 0xff) != 0) {
        obj->step_flags &= 0xff00;
        play_side_sound_at(obj, obj->side, 7);
        spawn_effect_at_object_offset(obj, 1, -0x2d, 0x2b);
        if (((Slot04aObj *)obj)->field_1cf == 0) {
            ((Slot04aObj *)obj)->field_1cf = 0xff;
            k = 8;
        } else {
            k = 0;
        }
        if ((apply_damage_to_other_return_defeated(obj, k, 0) & 0xff) != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 1);
            /* Stand-in, not recovered source: a second do-while wrapper, around the reload of r and the call that sounds 0x340, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
            do {
                r = &ref_other;
                play_sound_at(r->p, ref_other.p->side, 0x340);
            } while (0);
            goto tail;
        }
        /* Stand-in, not recovered source: a third do-while wrapper, around the reload of r, the increment of countdown and the call that sounds 0x30b, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
        do {
            r = &ref_other;
            obj->countdown++;
            play_sound_at(r->p, ref_other.p->side, 0x30b);
        } while (0);
        update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0) {
            goto idle;
        }
        r->p = obj->other;
        if (ref_other.p->field_15b == 0) {
            goto tail;
        }
        if ((drain_gauge_by_other_input(obj) & 0xff) != 0) {
            goto tail;
        }
        goto idle;
    } else {
        update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown == 0) {
            goto idle;
        }
        r->p = obj->other;
        if (ref_other.p->field_15b == 0) {
            goto tail;
        }
        if ((drain_gauge_by_other_input(obj) & 0xff) == 0) {
            goto idle;
        }
    }
tail:
    obj->state_3++;
    /* Stand-in, not recovered source: a fourth do-while wrapper, around the single statement that sounds 0x30d, reproduces the original's choice of registers. Without it this function differs from the original in 43 instruction slots. */
    do {
        play_sound_at(r->p, ref_other.p->side, 0x30d);
    } while (0);
    play_side_sound_at(obj, obj->side, 7);
    set_sequence_by_side_and_mode(obj, 0x41);
    return;
idle:
    update_sequence(obj);
}


