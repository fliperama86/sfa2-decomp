/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;
extern ObjectFn data_801ce49c_slot04_17[];

void func_801428e4(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80145d20(Object *object);
void func_801b57f0_slot04_17(Object *obj);
void func_801b3784_slot04_17(Object *obj);

void func_801b36e4_slot04_17(Object *obj) {
    int a = obj->vel_x;

    if (obj->field_0b == 0) {
        a = -a;
    }
    *(s32 *)&obj->pos_x_frac = a + *(s32 *)&obj->pos_x_frac;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if ((s32)obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3784_slot04_17(Object *obj) {
    obj->field_167 = (obj->field_12a >> 1) + 0xc;
    if (obj->field_12f == 0) {
        obj->field_12f = 0xff;
        data_80190468.p->field_6b = 5;
        func_80147000(obj);
    }
}

void func_801b37d0_slot04_17(Object *obj) {
    data_801ce49c_slot04_17[obj->state_3](obj);
}

void func_801b3810_slot04_17(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    func_80138b38(&game_state, obj);
    func_80145d20(obj);
    func_801307e0(obj, 0x40);
}
