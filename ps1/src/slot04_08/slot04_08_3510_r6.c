/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b3d2c_slot04_08(Object *obj) {
    Object *o;
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_17b = 0;
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            o = obj->other;
            o->field_15b = 1;
            obj->field_3a = obj->field_3a & 0xff00;
            func_80140770(obj, 2, 0xf, -0x200, 0, 1, 0);
            if ((s16)o->field_5c < 0) {
                obj->field_167 = 2;
                if (obj->field_49 != 0) {
                    obj->field_167 = 0x18;
                }
            }
        }
        func_80130efc(obj);
    }
}
