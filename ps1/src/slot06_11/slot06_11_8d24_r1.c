/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801e9da0_slot06_11[];
extern ObjectFn data_801e9db0_slot06_11[];
extern SeqRec *data_801e9d98_slot06_11[];

void func_801e8fa0_slot06_11(Object *obj);

void func_801e8d24_slot06_11(Object *obj) {
    data_801e9da0_slot06_11[obj->field_04](obj);
}

void func_801e8d64_slot06_11(Object *obj) {
    obj->field_01 = 0;
    obj->field_04++;
}

void func_801e8d78_slot06_11(Object *obj) {
    if (game_state.field_65 == 0) {
        data_801e9db0_slot06_11[obj->field_05](obj);
    }
}

void func_801e8dc4_slot06_11(Object *obj) {
    if ((func_80151184() & 0x1f) == 5) {
        obj->field_46 = (func_80151184() & 3) + 2;
        obj->field_3a = 1;
        obj->field_05++;
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8e2c_slot06_11(Object *obj) {
    obj->field_38 = (s16)obj->field_38 - 1;
    if ((s16)obj->field_38 == 0) {
        u16 t = obj->field_3a ^ 1;

        obj->field_3a = t;
        if (t != 0) {
            obj->field_46 = (s16)obj->field_46 - 1;
            if ((s16)obj->field_46 == 0) {
                obj->field_05++;
            }
        }
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8ea8_slot06_11(Object *obj) {
    u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
    u16 *q = (u16 *)(cam_obj + 0x4a);

    obj->field_05++;
    obj->field_46 = (u8)func_80151184();
    *p ^= 0x800;
    *q ^= 0x200;
}

void func_801e8f10_slot06_11(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        u16 *p = (u16 *)(data_801aa5d4 + 0x4a);
        u16 *q = (u16 *)(cam_obj + 0x4a);

        obj->field_05 = 0;
        *p ^= 0x800;
        *q ^= 0x200;
        obj->field_3a = 0;
        func_801e8fa0_slot06_11(obj);
    }
}

void func_801e8f80_slot06_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e8fa0_slot06_11(Object *obj) {
    func_8011fc54(obj, data_801e9d98_slot06_11, obj->field_3a);
    obj->field_38 = (func_80151184() & 3) + 1;
}
