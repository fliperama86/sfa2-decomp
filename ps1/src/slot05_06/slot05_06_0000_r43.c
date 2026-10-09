/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
extern ObjectFn data_801dd484_slot05_06[];

void func_801cba68_slot05_06(Object *obj) {
    Object *other;

    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        func_80140770(obj, 0, 0xc, 3, 0, 0, 0);
        if ((s16)other->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        }
        other->field_247 = 0;
        obj->field_17b = 0;
        func_801312b8(obj);
    }
}

void func_801cbb2c_slot05_06(Object *obj) {
    data_801dd484_slot05_06[obj->field_07](obj);
}
