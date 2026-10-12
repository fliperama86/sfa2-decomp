/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c0508_slot04_0a[];

void func_801b3e68_slot04_0a(Object *obj);
void func_801b3f30_slot04_0a(Object *obj);

void func_801b04a0_slot04_0a(Object *obj) {
    u16 a;
    int k;
    int d;
    int m = -0x100;

    if (obj->variant_index == 2) {
        if (obj->state_3 == 0) {
            a = obj->step_flags;
            if ((a & 0x7f00) == 0x100) {
                obj->step_flags = a & 0xff;
                func_801b3e68_slot04_0a(obj);
            }
            if ((obj->step_flags & 0xff) != 0) {
                obj->step_flags = obj->step_flags & m;
                obj->state_3 = obj->state_3 + 1;
                obj->active_state = 1;
                obj->countdown = obj->countdown & m;
                obj->pos_y = obj->ground_y - 0x20;
                func_801b3f30_slot04_0a(obj);
            }
        } else {
            k = (s16)obj->countdown;
            k += 1;
            obj->countdown = k & 0xff3f;
            k = get_random_byte() & 1;
            if (k != 0) {
                k = 0x40;
            }
            k += (u8)obj->countdown;
            k = data_801c0508_slot04_0a[k >> 2];
            d = *(u32 *)&obj->pos_y_frac;
            *(u32 *)&obj->pos_y_frac = d - (k << 8);
        }
    } else {
        if (obj->state_3 == 0) {
            obj->state_3 = obj->state_3 + 1;
        }
    }
    if ((s16)obj->countdown & 0xff00) {
        obj->countdown = (s16)obj->countdown - 0x100;
        if ((obj->countdown & 0xff00) == 0) {
            if (obj->side == 0) {
                game_state.side_bits_2 |= 1;
            } else {
                game_state.side_bits_2 |= 2;
            }
        }
    }
    update_sequence(obj);
    if (obj->step_flags & 0x80) {
        obj->step_flags = obj->step_flags & 0xff00;
    }
}
