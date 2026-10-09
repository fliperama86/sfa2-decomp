/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3c60_slot04_08[];
int func_80140cd8(Object *object, int a, int b);

void func_801b1f8c_slot04_08(Object *obj) {
    u16 t;
    Object *p;
    t = obj->field_3a;
    if ((u8)t != 0) {
        if ((t & 0x80) == 0) {
            obj->field_3a = t & 0xff00;
            game_state.field_63 = 0x18;
            func_80146960(obj);
        }
        obj->field_07 = obj->field_07 + 1;
        func_80140cd8(obj, *(s16 *)(data_801c3c60_slot04_08 + (obj->field_12a & 0xfe)), 0);
        p = obj->other;
        obj->field_46 = 0x18;
        if ((s16)p->field_5c < 0) {
            func_80120554(obj, obj->side, 0x319);
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x18;
                obj->field_255 = 6;
                game_state.field_6b = 0;
                func_80147000(obj);
            }
        } else {
            func_80120554(obj, obj->side, 0x319);
        }
    }
    func_80130efc(obj);
}

void func_801b2088_slot04_08(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_0b = obj->field_0b ^ 1;
        func_801307e0(obj, 0x3c);
    } else {
        func_80130efc(obj);
    }
}
