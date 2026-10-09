/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c4304_slot04_04[];

void func_801b2188_slot04_04(Object *obj) {
    u16 a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 10);
    func_80138ae8(&game_state, obj);
    if (obj->field_0b != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x + 0x18;
    } else {
        *(u16 *)&obj->pos_x = *(u16 *)&obj->pos_x - 0x18;
    }
    obj->field_4c = data_801c4304_slot04_04[obj->field_12a * 2];
    obj->field_54 = data_801c4304_slot04_04[obj->field_12a * 2 + 1];
    obj->field_50 = data_801c4304_slot04_04[obj->field_12a * 2 + 2];
    obj->field_58 = data_801c4304_slot04_04[obj->field_12a * 2 + 3];
    func_801204f4(obj, obj->side, 7);
    if (obj->field_49 != 0) {
        a = (obj->field_12a >> 1) + 0x4c;
    } else {
        a = obj->field_12a + 0x30;
    }
    func_801307e0(obj, a);
}

void func_801b22c8_slot04_04(Object *obj) {
    if ((obj->field_3a & 0x80) == 0) {
        obj->field_45 = 1;
        obj->field_07++;
    }
    func_80130efc(obj);
}
