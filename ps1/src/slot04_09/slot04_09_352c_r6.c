/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c7d64_slot04_09[];
void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b3ad8_slot04_09(Object *obj) {
    Object *other;
    u8 one = 1;
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
        return;
    }
    other = obj->other;
    obj->field_07++;
    obj->field_48 = 0;
    other->field_15b = one;
    other->field_260 = one;
    if (((Slot04aObj *)other)->field_5c < 0) {
        obj->field_48 = 0xff;
    }
    func_80140770(obj, 5, 0xd, data_801c7d64_slot04_09[(obj->field_12a & 0xfe) >> 1], 0, 1, 0);
    other = obj->other;
    if (((Slot04aObj *)other)->field_5c < 0) {
        obj->field_167 = (obj->field_12a >> 1) + 0xc;
        if (obj->field_48 == 0) {
            if (obj->field_0b != 0) {
                game_state.field_6b = one;
            } else {
                game_state.field_6b = 2;
            }
            func_80147000(obj);
        }
    }
    obj->field_50 = 0x40000;
    obj->field_54 = 0;
    obj->field_58 = -0x4000;
    if (obj->field_0b != 0) {
        obj->field_4c = 0x20000;
    } else {
        obj->field_4c = -0x20000;
    }
}
