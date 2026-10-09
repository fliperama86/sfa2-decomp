/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int rand(void);
void func_801e9d40_slot06_07(Object *obj);
void func_801e9d84_slot06_07(Object *obj);
Object *func_801ea010_slot06_07(void);

extern SequenceStep *data_801ee188_slot06_07[];
extern SequenceStep *data_801ee18c_slot06_07;
extern SequenceStep *data_801ee190_slot06_07;
extern u8 data_801ee1bc_slot06_07[];
extern ObjectFn data_801ee1cc_slot06_07[];
extern ObjectFn data_801ee1dc_slot06_07[];

void func_801e9cf4_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_801e9d40_slot06_07(obj);
    }
    func_80120028(obj);
}

void func_801e9d40_slot06_07(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        func_801e9d84_slot06_07(obj);
    } else {
        func_80131094(obj);
    }
}

void func_801e9d84_slot06_07(Object *obj) {
    u8 v = data_801ee1bc_slot06_07[rand() & 0xf];
    func_80130700(obj, data_801ee188_slot06_07[v]);
}

void func_801e9de0_slot06_07(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9e00_slot06_07(Object *obj) {
    data_801ee1cc_slot06_07[obj->field_04](obj);
}

void func_801e9e40_slot06_07(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_04++;
    func_80130700(obj, data_801ee18c_slot06_07);
}

void func_801e9e90_slot06_07(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        data_801ee1dc_slot06_07[obj->field_05](obj);
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801e9f04_slot06_07(Object *obj) {
    Object *p = func_801ea010_slot06_07();
    if (p != 0) {
        if ((u16)p->pos_x < (u16)(obj->pos_x - 0x20)) {
            obj->field_05++;
            func_80130700(obj, data_801ee190_slot06_07);
        }
    }
}

void func_801e9f74_slot06_07(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05++;
        obj->field_46 = func_80151184();
    }
}

void func_801e9fc8_slot06_07(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_05 = 0;
    }
}

void func_801e9ff0_slot06_07(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
