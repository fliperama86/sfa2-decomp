/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot04_08Rec3cb8 *data_801c3cb8_slot04_08[];
extern u16 data_801c3cc4_slot04_08[];

void func_80146960(Object *object);
int func_80140cd8(Object *obj, int a, int b);

void func_801b25a8_slot04_08(Object *obj) {
    Slot04_08Rec3cb8 *t = data_801c3cb8_slot04_08[obj->field_12a >> 1];
    int a = t[((Slot04aObj *)obj)->field_1c6].a;

    if (a < 0) {
        obj->field_07++;
    } else {
        obj->field_07 = 3;
        if (t[((Slot04aObj *)obj)->field_1c6].b != 0) {
            obj->field_0b ^= 1;
        }
        ((Slot04aObj *)obj)->field_1c6++;
        func_801307e0(obj, a);
    }
}

void func_801b2650_slot04_08(Object *obj) {

    u16 t = obj->field_3a;
    int m = -0x100;

    if ((t & 0xff) != 0) {
        obj->field_3a = t & m;
        game_state.field_63 = 0x18;
        func_80146960(obj);
        func_80120554(obj, ((Slot04aObj *)obj)->field_a6, 0x319);
        func_80140cd8(obj, (s16)(data_801c3cc4_slot04_08[(s16)(obj->field_12a * 2 + (((Slot04aObj *)obj)->field_1c6 + 0xffff))] | m), 0);
        obj->field_07++;
        obj->field_46 = 0x10;
    }
    func_80130efc(obj);
}
