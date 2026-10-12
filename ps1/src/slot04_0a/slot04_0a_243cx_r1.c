/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0670_slot04_0a[];

void func_801b243c_slot04_0a(Object *obj) {
    int m;
    int d;
    int a;
    Object *pl = &player_left;

    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        m = *(s16 *)box_margin;
        a = ((Slot04aObj *)obj)->offset_index;
        d = *(s16 *)((u8 *)data_801c0670_slot04_0a + (a & 0xfe));
        if (obj->facing_side == 0) {
            m += 0x180;
            d = -d;
        }
        obj->pos_x = d + m;
        if (pl->bound_push_flag != 0) {
            a = -1;
            if (obj->facing_side != 0) {
                a = 1;
            }
            *(u16 *)&player_left.pos_x += a;
            player_left.bound_push_flag = 0;
        }
        a = 0x2f;
        if (obj->field_49 != 0) {
            a = 0x4c;
        }
        func_801307e0(obj, a);
    }
}
