/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c7708_slot04_11[];
extern u32 data_801c719c_slot04_11[];

u8 get_random_table_byte_slot04_11(Object *obj);
void latch_pressed_bits_into_input_bits_slot04_11(Object *obj);

void start_jump_from_input_table_v2_slot04_11(Object *obj) {
    int a;
    int d;
    int m;
    int n;
    int j;
    int s;
    s32 x;
    s32 y;

    latch_pressed_bits_into_input_bits_slot04_11(obj);
    if ((obj->step_flags << 16) < 0) {
        a = *(u16 *)&((Slot04bObj *)obj)->input_bits;
        obj->state_3++;
        if (obj->mode_flag_2 != 0) {
            if (test_random_bit_in_table(obj, data_801c719c_slot04_11) == 0) {
                ref_other.p = obj->other;
                d = ref_other.p->pos_x - obj->pos_x;
                a = 0x40;
                if (d >= 0) {
                    a = 8;
                }
                d += 0x10;
                if (d < 0x21) {
                    a = 0x20;
                }
            } else {
                a = get_random_table_byte_slot04_11(obj);
            }
        }
        d = 1;
        a &= 0x68;
        if (a != 0) {
            if ((a & 0x20) == 0) {
                d = 0;
                obj->facing_side = 0;
                if ((a & 0x40) == 0) {
                    obj->facing_side = 1;
                }
            }
        }
        /* The locals j, n, m and s fix the order of these sums. With d << 2 written at its use five instruction slots differ, with m + n at its use twelve, with step_count_2++ eight. */
        j = d << 2;
        n = obj->step_count_2 + 8;
        m = obj->direction_index * 6;
        *(u16 *)&((Slot04bObj *)obj)->input_bits = 0;
        obj->step_count_2 = obj->step_count_2 + 1;
        a = j + m;
        x = data_801c7708_slot04_11[a];
        y = data_801c7708_slot04_11[a + 1];
        obj->vel_y = data_801c7708_slot04_11[a + 2];
        obj->accel_y = data_801c7708_slot04_11[a + 3];
        s = m + n;
        if (obj->facing_side == 0) {
            x = -x;
            y = -y;
        }
        obj->vel_x = x;
        obj->accel_x = y;
        set_sequence_by_side_and_mode(obj, d + data_801c7708_slot04_11[s]);
    } else {
        update_sequence(obj);
    }
}
