/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u8 data_801ac6a8[];

void func_80134624(Object *object) {
    s16 index = data_801a27d0;
    u8 *base = data_801ac6a8;

    if (object->field_a0 >= 0x90) {
        object->field_a0 = 0x90;
    }
    if (object->field_03 == 0) {
        if (data_80198098 == 0) {
            *(u16 *)&base[(index << 4) + 0x18c] = 0xa6 - object->field_a0;
            *(u16 *)&base[(index << 4) + 0x190] = object->field_a0;
        } else {
            if (object->field_a0 >= 0x48) {
                object->field_a0 = 0x48;
            }
            *(u16 *)&base[(index << 4) + 0x18c] = 0xa6 - object->field_a0 * 2;
            *(u16 *)&base[(index << 4) + 0x190] = object->field_a0 * 2;
        }
        func_8015bf34(data_801987c8 + 0x1c, &base[(index << 4) + 0x184]);
    } else {
        if (data_8019842c == 0) {
            *(u16 *)&base[(index << 4) + 0x1b0] = object->field_a0;
        } else {
            if (object->field_a0 >= 0x48) {
                object->field_a0 = 0x48;
            }
            *(u16 *)&base[(index << 4) + 0x1b0] = object->field_a0 * 2;
        }
        func_8015bf34(data_801987c8 + 0x1c, &base[(index << 4) + 0x1a4]);
    }
    func_801347b4(7, object->sequence->field_04, object->pos_x, object->pos_y, 7, 0x19);
}
