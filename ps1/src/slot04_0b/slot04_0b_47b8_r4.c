/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c28_slot04_0b(Object *obj) {
    int lim;
    u8 a;
    u16 p;
    u16 q;
    func_80131094(obj);
    lim = ref_other.p->field_70 << 16;
    if (obj->state_2 == 0) {
        obj->vel_x += (s16)obj->field_5c << 8;
        obj->accel_x -= (s16)obj->field_70 << 8;
        obj->field_70 += obj->field_26;
        if (!(obj->accel_x < lim)) {
            obj->state_2 = 8;
            obj->accel_x = lim;
        }
    }
    if (obj->state_3 == 0) {
        obj->vel_y += (s16)obj->field_5e << 8;
        obj->accel_y -= (s16)obj->field_76 << 8;
        obj->field_76 += obj->field_46;
        if (!(obj->accel_y < lim)) {
            obj->state_3 = 8;
            obj->accel_y = lim;
        }
    }
    a = game_state.field_1d;
    if (!(a & 1)) {
        p = ((u16 *)&obj->vel_x)[1];
        q = ((u16 *)&obj->accel_x)[1];
        a = obj->state_2;
    } else {
        p = ((u16 *)&obj->vel_y)[1];
        q = ((u16 *)&obj->accel_y)[1];
        a = obj->state_3;
    }
    obj->pos_y = q;
    obj->pos_x = p;
    if ((*(u32 *)&obj->state_0 & 0xffff0000) == 0x8080000) {
        obj->state_0++;
    }
    if (a == 0) {
        func_8011ffdc(obj);
    }
}
