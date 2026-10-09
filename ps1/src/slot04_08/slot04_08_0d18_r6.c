/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c3c04_slot04_08[];
extern u16 data_801c3bfc_slot04_08[];
extern u16 data_801c3e98_slot04_08;

void func_801b1424_slot04_08(Object *obj) {
    u16 d;
    int a;

    obj->field_17b = 1;
    obj->field_07++;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    obj->field_4c = data_801c3c04_slot04_08[obj->field_12a >> 1];
    obj->field_54 = 0xffff4000;
    d = data_801c3bfc_slot04_08[obj->field_12a >> 1];
    if (obj->field_0b == 0) {
        d = -d;
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    data_801c3e98_slot04_08 = d + (u16)obj->pos_x;
    a = 0x1d;
    if (obj->field_49 != 0) {
        a = 0x65;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
