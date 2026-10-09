/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c3d48_slot04_08[];
extern u16 data_801c3e98_slot04_08;

void func_801b4298_slot04_08(Object *obj);
void func_801b2cec_slot04_08(Object *obj);

void func_801b2bf8_slot04_08(Object *obj) {
    u16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if ((t & 0xff) != 0) {
        obj->field_3a = t & 0xff00;
        if (obj->field_4b == 0) {
            obj->field_165 = 0xff;
        } else {
            obj->field_165 = 1;
        }
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x31c);
        func_801483a4(obj, -0xa, 0x72);
    }
    if (obj->field_3a != 0) {
        u8 i = 0;

        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c3d48_slot04_08[i];
        func_801307e0(obj, (obj->field_12a >> 1) + 0x55);
        func_801b2cec_slot04_08(obj);
    }
}

void func_801b2cec_slot04_08(Object *obj) {
    int a;

    if ((u8)obj->field_3a != 0) {
        obj->field_4c = 0x90000;
        obj->field_54 = -0x4000;
        obj->field_07++;
        obj->field_67 = 0;
        a = 0xc0;
        if (obj->field_0b == 0) {
            a = -0xc0;
            obj->field_4c = -obj->field_4c;
            obj->field_54 = -obj->field_54;
        }
        data_801c3e98_slot04_08 = a + obj->pos_x;
        obj->field_46 = 0;
        func_801b4298_slot04_08(obj);
    }
    func_80130efc(obj);
}
