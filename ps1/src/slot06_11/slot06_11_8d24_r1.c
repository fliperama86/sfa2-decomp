/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801e9da0_slot06_11[];
extern ObjectFn data_801e9db0_slot06_11[];
extern SeqRec *data_801e9d98_slot06_11[];

void func_801e8fa0_slot06_11(Object *obj);

void func_801e8d24_slot06_11(Object *obj) {
    data_801e9da0_slot06_11[obj->state_0](obj);
}

void func_801e8d64_slot06_11(Object *obj) {
    obj->field_01 = 0;
    obj->state_0++;
}

void func_801e8d78_slot06_11(Object *obj) {
    if (game_state.field_65 == 0) {
        data_801e9db0_slot06_11[obj->state_1](obj);
    }
}

void func_801e8dc4_slot06_11(Object *obj) {
    if ((get_random_byte() & 0x1f) == 5) {
        obj->field_46 = (get_random_byte() & 3) + 2;
        obj->step_flags = 1;
        obj->state_1++;
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8e2c_slot06_11(Object *obj) {
    obj->step_timer = (s16)obj->step_timer - 1;
    if ((s16)obj->step_timer == 0) {
        u16 t = obj->step_flags ^ 1;

        obj->step_flags = t;
        if (t != 0) {
            obj->field_46 = (s16)obj->field_46 - 1;
            if ((s16)obj->field_46 == 0) {
                obj->state_1++;
            }
        }
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8ea8_slot06_11(Object *obj) {
    u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
    u16 *q = (u16 *)(cam_obj + 0x4a);

    obj->state_1++;
    obj->field_46 = (u8)get_random_byte();
    *p ^= 0x800;
    *q ^= 0x200;
}

void func_801e8f10_slot06_11(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
        u16 *q = (u16 *)(cam_obj + 0x4a);

        obj->state_1 = 0;
        *p ^= 0x800;
        *q ^= 0x200;
        obj->step_flags = 0;
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8f80_slot06_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e8fa0_slot06_11(Object *obj) {
    func_8011fc54(obj, data_801e9d98_slot06_11, obj->step_flags);
    obj->step_timer = (get_random_byte() & 3) + 1;
}
