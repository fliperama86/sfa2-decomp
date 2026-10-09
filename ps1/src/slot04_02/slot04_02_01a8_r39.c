/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c64f4_slot04_02[];
extern u16 data_801c64f8_slot04_02[];

void func_801b4d28_slot04_02(Object *obj) {
    int i;

    if ((game_state.field_32 & 3) == 0) {
        i = 1;
        if ((u8)obj->field_3a != 0) {
            i = 2;
        }
        for (; i >= 0; i--) {
            u16 a;

            if (!func_80148e84(obj)) {
                break;
            }
            if ((u8)obj->field_3a != 0) {
                a = data_801c64f8_slot04_02[i];
            } else {
                a = data_801c64f4_slot04_02[i];
            }
            if (obj->field_0b != 0) {
                a = -a;
            }
            ref_other.p->pos_x = a + ref_other.p->pos_x;
        }
    }
}

void func_801b4e10_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_80131468(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
