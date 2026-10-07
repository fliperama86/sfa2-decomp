/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e9f7c_slot06_08(Object *obj);
void func_801e9fe8_slot06_08(Object *obj);

extern SequenceStep *data_801ed480_slot06_08[];
extern SequenceStep *data_801ed498_slot06_08[];
extern SequenceStep *data_801ed4b4_slot06_08[];
extern ObjectFn data_801ed4d4_slot06_08[];

void func_801e9f10_slot06_08(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->field_03 == 0) {
            func_801e9f7c_slot06_08(obj);
            return;
        }
        func_801e9fe8_slot06_08(obj);
    }
    func_80120028(obj);
}

void func_801e9f7c_slot06_08(Object *obj) {
    if (obj->field_05 != 0) {
        if ((s16)obj->field_3a & 0x8000) {
            obj->field_05 = 0;
            func_80130700(obj, data_801ed498_slot06_08[0]);
        }
    }
    func_80131094(obj);
    func_80120028(obj);
}

void func_801e9fe8_slot06_08(Object *obj) {
    if (obj->field_05 == 0) {
        if (game_state.field_47 != 0) {
            if (game_state.field_5c == 0) {
                obj->field_05++;
                func_80130700(obj, data_801ed4b4_slot06_08[0]);
            }
        }
    }
    func_80131094(obj);
}

void func_801ea06c_slot06_08(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea08c_slot06_08(Object *obj) {
    data_801ed4d4_slot06_08[obj->field_04](obj);
}

void func_801ea0cc_slot06_08(Object *obj) {
    s16 idx;
    obj->field_81 = 4;
    obj->field_04++;
    idx = game_state.field_42;
    if (obj->field_03 != 0) {
        idx = ~idx;
    }
    if (idx & 1) {
        obj->field_04 = 2;
    }
    idx = 10;
    obj->field_0c = 0;
    obj->field_0a = 1;
    obj->field_0f = 1;
    if (obj->field_03 != 0) {
        idx = 12;
    }
    func_80130700(obj, data_801ed480_slot06_08[idx]);
}
