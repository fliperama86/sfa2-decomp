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
    obj->state_0 = obj->state_0 + 1;
    func_80027600_slot28(obj);
    func_800275c4_slot28(obj);
    func_800274b8_slot28(obj);
}

void func_800274b8_slot28(Object *obj) {
    func_80130768(obj, data_80050ca0_slot28[get_random_byte() & 3], (SequenceStep **)obj->box_tables);
}

void func_80027500_slot28(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= 0xb0) {
        func_80027600_slot28(obj);
        func_800275c4_slot28(obj);
        func_800274b8_slot28(obj);
    }
    func_80131094(obj);
    obj->field_01 = 1;
}
