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
    if (obj->field_06 == 0) {
        obj->field_4c += (s16)obj->field_5c << 8;
        obj->field_54 -= (s16)obj->field_70 << 8;
        obj->field_70 += obj->field_26;
        if (!(obj->field_54 < lim)) {
            obj->field_06 = 8;
            obj->field_54 = lim;
        }
    }
    if (obj->field_07 == 0) {
        obj->field_50 += (s16)obj->field_5e << 8;
        obj->field_58 -= (s16)obj->field_76 << 8;
        obj->field_76 += obj->field_46;
        if (!(obj->field_58 < lim)) {
            obj->field_07 = 8;
            obj->field_58 = lim;
        }
    }
    a = game_state.field_1d;
    if (!(a & 1)) {
        p = ((u16 *)&obj->field_4c)[1];
        q = ((u16 *)&obj->field_54)[1];
        a = obj->field_06;
    } else {
        p = ((u16 *)&obj->field_50)[1];
        q = ((u16 *)&obj->field_58)[1];
        a = obj->field_07;
    }
    obj->pos_y = q;
    obj->pos_x = p;
    if ((*(u32 *)&obj->field_04 & 0xffff0000) == 0x8080000) {
        obj->field_04++;
    }
    if (a == 0) {
        func_8011ffdc(obj);
    }
}
