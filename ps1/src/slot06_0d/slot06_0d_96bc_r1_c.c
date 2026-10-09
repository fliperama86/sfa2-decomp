/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea540_slot06_0d[];
extern ObjectFn data_801ea550_slot06_0d[];
extern u8 data_801ea558_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

void func_801e9e30_slot06_0d(Object *obj);
void func_8011f38c(Object *o);

void func_801e9c10_slot06_0d(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801e9c30_slot06_0d(Object *obj) {
    data_801ea540_slot06_0d[obj->field_04](obj);
}

void func_801e9c70_slot06_0d(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_04 = 1;
    obj->field_01 = 1;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_09 = 0x1a;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->field_26 = p->field_26;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    func_801e9e30_slot06_0d(obj);
}

void func_801e9cf0_slot06_0d(Object *obj) {
    Object *p = obj->field_3c;
    if (game_state.field_65 == 0) {
        data_801ea550_slot06_0d[obj->field_05](obj);
    }
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
}

void func_801e9d70_slot06_0d(Object *obj) {
    int v;
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_05 = obj->field_05 + 1;
        v = func_80151184() & 0x30;
        v |= 0x40;
        obj->field_46 = v << (func_80151184() & 3);
    }
    func_80131094(obj);
}

void func_801e9de4_slot06_0d(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_05 = 0;
        func_801e9e30_slot06_0d(obj);
    } else {
        func_80131094(obj);
    }
}

void func_801e9e30_slot06_0d(Object *obj) {
    u8 i = obj->field_03 + 5;
    if (obj->field_03 == 2) {
        i = data_801ea558_slot06_0d[func_80151184() & 0xf];
    }
    func_80130768(obj, i, data_801eb1b0_slot06_0d);
}

void func_801e9e9c_slot06_0d(Object *obj) {
    Object *p = obj->field_3c;
    if (p->field_28 == (u32)obj) {
        p->field_28 = 0;
    } else if (*(Object **)&p->field_2c == obj) {
        *(u32 *)&p->field_2c = 0;
    } else if (((Slot06Obj *)p)->field_30 == (s32)obj) {
        ((Slot06Obj *)p)->field_30 = 0;
    } else if (p->field_34 == (u32)obj) {
        p->field_34 = 0;
    }
    func_8011f38c(obj);
}
