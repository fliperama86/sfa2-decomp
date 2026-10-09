/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80050ca0_slot28[];
void func_800274b8_slot28(Object *obj);
void func_800275c4_slot28(Object *obj);
void func_80027600_slot28(Object *obj);

void func_8002746c_slot28(Object *obj) {
    obj->field_0f = 1;
    obj->field_04 = obj->field_04 + 1;
    func_80027600_slot28(obj);
    func_800275c4_slot28(obj);
    func_800274b8_slot28(obj);
}

void func_800274b8_slot28(Object *obj) {
    func_80130768(obj, data_80050ca0_slot28[func_80151184() & 3], (SequenceStep **)obj->box_tables);
}

void func_80027500_slot28(Object *obj) {
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->pos_y >= 0xb0) {
        func_80027600_slot28(obj);
        func_800275c4_slot28(obj);
        func_800274b8_slot28(obj);
    }
    func_80131094(obj);
    obj->field_01 = 1;
}
