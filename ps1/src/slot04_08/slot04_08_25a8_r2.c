/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3cc4_slot04_08[];

int func_80140cd8(Object *obj, int a, int b);
void func_801b25a8_slot04_08(Object *obj);

void func_801b2720_slot04_08(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        func_801b25a8_slot04_08(obj);
    }
}

void func_801b275c_slot04_08(Object *obj) {
    int a = 0x64;

    obj->field_07++;
    obj->field_0b ^= 1;
    if (obj->field_12a != 0) {
        a = 0x78;
    }
    func_801307e0(obj, a);
}

void func_801b27a4_slot04_08(Object *obj) {

    u16 t = obj->field_3a;

    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        game_state.field_63 = 0x18;
        func_80146960(obj);
        if ((u8)func_80140cd8(obj, (s16)data_801c3cc4_slot04_08[((Slot04aObj *)obj)->field_1c6 + (obj->field_12a << 1)], 0) != 0) {
            game_state.field_6b = 4;
            func_80147000(obj);
        }
        obj->field_46 = 0x10;
        obj->field_07++;
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x319);
    }
    func_80130efc(obj);
}
