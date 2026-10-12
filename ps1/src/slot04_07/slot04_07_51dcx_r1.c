/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c2134_slot04_07[];

void func_801b51dc_slot04_07(Object *obj) {
    u16 *p;
    u16 v;
    s16 k;
    u8 r = get_random_byte() & 7;
    obj->field_48 = r;
    p = &data_801c2134_slot04_07[r * 4];
    if (obj->field_0b == 0) {
        v = *p++;
        k = -8;
        *(s16 *)&obj->accel_x = k;
        *(u16 *)&obj->vel_x = v;
        v = *p++;
        *(s16 *)((u8 *)&obj->accel_x + 2) = 0;
    } else {
        v = *p++;
        k = 8;
        *(s16 *)&obj->accel_x = k;
        *(u16 *)&obj->vel_x = -v;
        v = -*p++;
        *(s16 *)((u8 *)&obj->accel_x + 2) = 0;
    }
    *(u16 *)((u8 *)&obj->vel_x + 2) = v;
    *(u16 *)&obj->vel_y = p[0];
    *(s16 *)&obj->accel_y = -0x30;
    *(u16 *)((u8 *)&obj->vel_y + 2) = p[1];
    *(s16 *)((u8 *)&obj->accel_y + 2) = -0x38;
}
