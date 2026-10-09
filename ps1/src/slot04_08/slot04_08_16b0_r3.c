/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c3e98_slot04_08;
extern u8 data_801c3c24_slot04_08[];

void func_801b4298_slot04_08(Object *obj);
void func_80138ae8(GameState *state, Object *obj);

void func_801b19d0_slot04_08(Object *obj) {
    u16 a;
    int n;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    *(s32 *)&obj->field_4c = 0x80000;
    obj->field_54 = -0xc000;
    a = *(u16 *)(data_801c3c24_slot04_08 + (obj->field_129 & 0xfe));
    if (obj->field_0b == 0) {
        a = -a;
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    data_801c3e98_slot04_08 = a + obj->pos_x;
    n = 0x23;
    if (obj->field_49 != 0) {
        n = 0x6b;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + n);
}

void func_801b1ab0_slot04_08(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_46 = 0;
        obj->field_07++;
        func_801b4298_slot04_08(obj);
    }
    func_80130efc(obj);
}
