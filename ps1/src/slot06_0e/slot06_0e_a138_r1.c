/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ef40c_slot06_0e[];
extern SequenceStep *data_801ef410_slot06_0e[];
extern SequenceStep *data_801ef414_slot06_0e[];
extern SequenceStep *data_801ef424_slot06_0e;
extern SequenceStep *data_801ef42c_slot06_0e;
extern SequenceStep *data_801ef430_slot06_0e[];
extern ObjectFn data_801ef434_slot06_0e[];
extern ObjectFn data_801ef444_slot06_0e[];
extern ObjectFn data_801ef44c_slot06_0e[];
extern ObjectFn data_801ef45c_slot06_0e[];
extern ObjectFn data_801ef464_slot06_0e[];
extern ObjectFn data_801ef474_slot06_0e[];
extern ObjectFn data_801ef484_slot06_0e[];

Object *func_8011f32c(void);
void func_8011f38c(Object *o);

void func_801ea138_slot06_0e(Object *obj) {
    data_801ef434_slot06_0e[obj->field_04](obj);
}

void func_801ea178_slot06_0e(Object *obj) {
    obj->field_0f = 1;
    obj->field_04 = 1;
    obj->field_14 = 0;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    obj->field_a0 = 0;
    func_80130700(obj, data_801ef414_slot06_0e[0]);
}

void func_801ea1c4_slot06_0e(Object *obj) {
    Slot06Layer *layer = (Slot06Layer *)data_801aa5d4;
    u8 side;
    s16 x;
    if (layer->field_12 > layer->field_0a) {
        side = 1;
        x = 0x381;
        if (obj->field_a0 != 1) {
            func_80130700(obj, data_801ef42c_slot06_0e);
        }
    } else {
        side = 0;
        x = 0x17f;
        if (obj->field_a0 != 0) {
            func_80130700(obj, data_801ef414_slot06_0e[0]);
        }
    }
    obj->pos_x = x;
    obj->field_0b = side;
    data_801ef444_slot06_0e[obj->field_05](obj);
    func_80120028(obj);
    obj->field_a0 = obj->field_0b;
}

void func_801ea29c_slot06_0e(Object *obj) {
    int t;
    s16 d;
    s16 u;
    t = game_state.field_65 | game_state.field_74;
    if ((t | ((Slot06Layer *)cam_obj)->field_8b) == 0) {
        if (data_801aa548[1] == 2) {
            obj->field_05++;
        }
        d = 0xf8 - (u16)obj->pos_y;
        u = d - 1;
        obj->pos_y = 0xf8 - u;
        if ((u16)u < 0x79) {
            u = d + 0xf;
            obj->pos_y = 0xf8 - u;
            func_80131094(obj);
        }
    }
}

void func_801ea338_slot06_0e(Object *obj) {
}

void func_801ea340_slot06_0e(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea360_slot06_0e(Object *obj) {
    data_801ef44c_slot06_0e[obj->field_04](obj);
}

void func_801ea3a0_slot06_0e(Object *obj) {
    Object *c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x15;
        c->field_0c = obj->field_0c;
        c->field_0e = obj->field_0e;
        c->field_26 = obj->field_26;
        c->field_1c = obj->field_1c;
        c->field_3c = obj;
        c->field_81 = 4;
        c->field_09 = obj->field_09;
        *(Object **)&obj->field_2c = c;
    }
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->field_0c = 0;
    obj->field_0d = 0;
    obj->field_81 = 4;
    obj->field_04 = obj->field_04 + 1;
    *(u32 *)&obj->field_14 &= 0xffff0000;
    func_80130700(obj, data_801ef40c_slot06_0e[0]);
}

void func_801ea470_slot06_0e(Object *obj) {
    data_801ef45c_slot06_0e[obj->field_05](obj);
    func_80120028(obj);
}

void func_801ea4c4_slot06_0e(Object *obj) {
    s16 y;
    int t;
    t = game_state.field_65 | game_state.field_74;
    if ((t | ((Slot06Layer *)cam_obj)->field_8b) == 0) {
        if (data_801aa548[1] == 2) {
            obj->field_05++;
        }
        y = (u16)obj->pos_y;
        y += 1;
        obj->pos_y = y;
        if (y >= 0x130) {
            obj->pos_y = -0xd7;
        }
    }
}

void func_801ea544_slot06_0e(Object *obj) {
}

void func_801ea54c_slot06_0e(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801ea56c_slot06_0e(Object *obj) {
    data_801ef464_slot06_0e[obj->field_04](obj);
}

void func_801ea5ac_slot06_0e(Object *obj) {
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_04 = 1;
    obj->field_01 = 1;
    obj->field_81 = 4;
    obj->field_0f = 1;
    func_80130700(obj, data_801ef410_slot06_0e[0]);
}

void func_801ea5f4_slot06_0e(Object *obj) {
    Slot06Layer *layer = (Slot06Layer *)data_801aa5d4;
    Object *p = obj->field_3c;
    if (*(u32 *)&layer->field_10 > *(u32 *)&layer->field_08) {
        obj->field_0b = 1;
        obj->pos_x = 0x3b0;
        func_80130700(obj, data_801ef430_slot06_0e[0]);
    } else {
        obj->field_0b = 0;
        obj->pos_x = 0x150;
        func_80130700(obj, data_801ef410_slot06_0e[0]);
    }
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y - 8;
}

void func_801ea68c_slot06_0e(Object *obj) {
    func_8011f38c(obj);
}

void func_801ea6ac_slot06_0e(Object *obj) {
    data_801ef474_slot06_0e[obj->field_04](obj);
}

void func_801ea6ec_slot06_0e(Object *obj) {
    Object *p = obj->field_3c;
    obj->field_04 = obj->field_04 + 1;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->field_26 = p->field_26;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    obj->field_81 = 4;
    func_80130700(obj, data_801ef424_slot06_0e);
}

void func_801ea778_slot06_0e(Object *obj) {
    Object *p = obj->field_3c;
    if (obj->field_01 != 0) {
        *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
        *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
        func_80131094(obj);
    }
}

void func_801ea7b8_slot06_0e(Object *obj) {
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

void func_801ea83c_slot06_0e(Object *obj) {
    data_801ef484_slot06_0e[obj->field_04](obj);
}
