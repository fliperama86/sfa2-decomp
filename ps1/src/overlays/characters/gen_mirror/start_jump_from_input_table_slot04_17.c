/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

/* Functions of this image that the shared headers do not declare, as the image's units declare them. */
void latch_pressed_bits_into_input_bits_slot04_17(Object *obj);
u8 mirror_value_by_facing_side_slot04_17(Object *obj, int a);
u8 get_random_table_byte_slot04_17(Object *obj);
Object *get_table_object_by_field_slot04_17(Object *obj);

extern u32 data_801ce0d0_slot04_17[];
extern s32 data_801ce57c_slot04_17[];

/*
 * What it does (inferred, not an original name): a character's step handler
 * of a one-key move. It first lets the input collector (func_801b4970) run.
 * While the current animation step is not the last one (bit 15 of step_flags
 * clear) it only calls update_sequence. Otherwise it advances state_3 by 3
 * and forms a direction code. For an object with mode_flag_2 clear the code
 * is the halfword input_bits (the collector's result). For one with
 * mode_flag_2 set the code comes from a table test (test_random_bit_in_table): when it
 * answers yes, from a random pick (func_801b49e8); when no, from the
 * distance of the other object's field_12 to this one's (8 when 0x100 or
 * more, 0x20 from 0x80, else 0x40), and ref_other is set to field_40. The
 * code goes through func_801b49b8 (which mirrors it when facing_side is set).
 * A variant k is then 0 when bit 3 (value 8) of the code is set, 2 when 0x40 is
 * set, else 1. The move reads four words of a table at row direction_index, entry
 * k (eight words to a row): speed words for vel_x, accel_x, vel_y,
 * accel_y, with the first two negated when facing_side is clear. It counts
 * step_count_2 up, clears input_bits, and starts the sequence k plus the table
 * word at row direction_index, offset step_count_2 (old value) plus 12.
 */
void start_jump_from_input_table_slot04_17(Object *obj) {
    int c;
    int k;
    int d;
    int j;
    int i;
    s32 x;
    s32 y;

    latch_pressed_bits_into_input_bits_slot04_17(obj);
    /* Bit 15 of step_flags, tested as the sign of the word shifted up by 16 (a shift of an unsigned value; its conversion to a signed word is implementation-defined and keeps the bit on this compiler and on the PC's). With (s16)obj->step_flags < 0 this function differs from the original in 4 instruction slots, with ((u32)obj->step_flags << 16) >> 31 in 2, with (obj->step_flags & 0x8000) == 0 and an early return in 11. */
    if ((s32)((u32)obj->step_flags << 16) < 0) {
        c = *(u16 *)&((Slot04bObj *)obj)->input_bits;
        obj->state_3 += 3;
        if (obj->mode_flag_2 != 0) {
            if (test_random_bit_in_table(obj, data_801ce0d0_slot04_17) == 0) {
                get_table_object_by_field_slot04_17(obj);
                ref_other.p = obj->other;
                d = ref_other.p->pos_x - obj->pos_x;
                c = 8;
                if (d < 0x100) {
                    c = 0x20;
                    if (d < 0x80) {
                        c = 0x40;
                    }
                }
            } else {
                c = get_random_table_byte_slot04_17(obj);
            }
        }
        c = mirror_value_by_facing_side_slot04_17(obj, c);
        d = 0;
        if ((c & 8) == 0) {
            d = 2;
            if ((c & 0x40) == 0) {
                d = 1;
            }
        }
        /* The do-while wrapper reproduces the original's order of instructions: the compiler does not move an instruction across its ends. It emits no instruction of its own. It is a stand-in for whatever grouped these statements in the original source, not an explanation. */
        /* Without it this function differs from the original in 32 instruction slots. */
        do {
            i = obj->direction_index;
            i <<= 3;
            k = d << 2;
        } while (0);
        c = k + i;
        j = obj->step_count_2;
        obj->step_count_2++;
        x = data_801ce57c_slot04_17[c];
        y = data_801ce57c_slot04_17[c + 1];
        obj->vel_y = data_801ce57c_slot04_17[c + 2];
        obj->accel_y = data_801ce57c_slot04_17[c + 3];
        j += 12;
        i += j;
        if (obj->facing_side == 0) {
            x = -x;
            y = -y;
        }
        *(u16 *)&((Slot04bObj *)obj)->input_bits = 0;
        obj->vel_x = x;
        obj->accel_x = y;
        set_sequence_by_side_and_mode(obj, d + data_801ce57c_slot04_17[i]);
    } else {
        update_sequence(obj);
    }
}
