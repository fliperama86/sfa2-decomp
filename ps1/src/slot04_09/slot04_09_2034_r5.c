/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c7cb8_slot04_09[];
void func_801b27f8_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b2548_slot04_09(Object *obj) {
    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if (obj->field_50 >= 0 && (s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_0b ^= 1;
        func_801307e0(obj, 0x49);
    }
}

void func_801b25cc_slot04_09(Object *obj) {
    Object *o;

    func_801b27f8_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        o = obj->other;
        o->field_15b = 1;
        game_state.field_63 = 0x18;
        func_80140770(obj, 5, 10, *(s16 *)((u8 *)data_801c7cb8_slot04_09 + (obj->field_12a & 0xfe)), 0, 1, 0);
        o = obj->other;
        if ((s16)o->field_5c < 0) {
            obj->field_167 = 0x19;
            obj->field_255 = 6;
            game_state.field_6b = 0;
            func_80147000(obj);
        }
    }
}
