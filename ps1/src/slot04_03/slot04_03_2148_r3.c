/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0708_slot04_03[];
void func_801428e4(Object *object);
void func_80145d20(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_801483a4(Object *object, int a_arg, int b_arg);

void func_801b232c_slot04_03(Object *obj) {
    obj->field_46 = 0x300;
    obj->field_07++;
    func_80145d20(obj);
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x29);
}

void func_801b2394_slot04_03(Object *obj) {
    func_80130efc(obj);
    if (obj->field_3a != 0) {
        obj->field_07++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        func_80120554(obj, obj->side, 0x31c);
        func_801483a4(obj, 0x34, 0x4c);
    }
}

void func_801b2410_slot04_03(Object *obj) {
    func_80130efc(obj);
    if (((s16)obj->field_3a & 0xff00) != 0) {
        u8 i = 0;

        obj->field_07++;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            i = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c0708_slot04_03[i];
        func_801204f4(obj, obj->side, 6);
        func_801204f4(obj, obj->side, 0xc);
    }
}
