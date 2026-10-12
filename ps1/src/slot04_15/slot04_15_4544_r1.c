/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c3cb0_slot04_15[];
extern ObjectFn data_801c3cbc_slot04_15[];
extern ObjectFn data_801c3cc4_slot04_15[];
extern ObjectFn data_801c3cd0_slot04_15[];
extern ObjectFn data_801c3cdc_slot04_15[];
extern ObjectFn data_801c3ce8_slot04_15[];
extern ObjectFn data_801c3cf4_slot04_15[];
extern SequenceStep *data_801c0fc4_slot04_15[];

void func_80131468(Object *object);
void func_80130dc0(Object *object);
void func_8011f240(Slab172 *s);
void func_801b4c10_slot04_15(Object *obj);
void func_801b4678_slot04_15(Object *obj);
void func_801b49d8_slot04_15(Object *obj);
void func_801b46b8_slot04_15(Object *obj);

void func_801b4544_slot04_15(Object *obj) {
    Object *p;
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac += obj->vel_y;
    obj->vel_y += obj->accel_y;
    p = obj->field_3c;
    t = p->ground_y;
    if (t > obj->pos_y) {
        func_80131094(obj);
    } else {
        obj->state_1 = obj->state_1 + 1;
        obj->pos_y = (u16)p->ground_y;
        func_80130768(obj, 1, data_801c0fc4_slot04_15);
    }
}

void func_801b45e4_slot04_15(Object *obj) {
    func_80131094(obj);
}

void func_801b4604_slot04_15(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
}

void func_801b4618_slot04_15(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b4638_slot04_15(Object *obj) {
    if (obj->field_128 != 0) {
        func_801b4c10_slot04_15(obj);
    } else {
        func_801b4678_slot04_15(obj);
    }
}

void func_801b4678_slot04_15(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b49d8_slot04_15(obj);
    } else {
        func_801b46b8_slot04_15(obj);
    }
}

void func_801b46b8_slot04_15(Object *obj) {
    data_801c3cb0_slot04_15[obj->field_12a >> 1](obj);
}

void func_801b46fc_slot04_15(Object *object) {
    data_801c3cbc_slot04_15[object->state_3](object);
}

void func_801b473c_slot04_15(Object *obj) {
    obj->state_3++;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b47cc_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4830_slot04_15(Object *object) {
    data_801c3cc4_slot04_15[object->state_3](object);
}

void func_801b4870_slot04_15(Object *obj) {
    u8 one;

    obj->state_3++;
    if (obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        ((Slot04bObj *)obj)->field_278 = one;
    }
}

void func_801b48f4_slot04_15(Object *obj) {
    s16 d;

    if (obj->step_flags & 0xff) {
        obj->state_3++;
        d = -0x18;
        if (obj->field_0b != 0) {
            d = 0x18;
        }
        *(s32 *)&obj->pos_x_frac += d;
    }
    if (func_80149b80(obj) & 0xff) {
        obj->state_3 = 0;
    }
    update_sequence(obj);
}

void func_801b4974_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b49d8_slot04_15(Object *obj) {
    data_801c3cd0_slot04_15[obj->field_12a >> 1](obj);
}

void func_801b4a1c_slot04_15(Object *object) {
    data_801c3cdc_slot04_15[object->state_3](object);
}

void func_801b4a5c_slot04_15(Object *obj) {
    u8 one;

    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x11, 0x14) & 0xff) {
            obj->state_0 = 1;
            obj->state_1 = 2;
            obj->state_2 = 0;
            obj->state_3 = 0;
        }
    } else {
        one = 1;
        obj->field_159 = one;
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80130ec0(obj);
            ((Slot04bObj *)obj)->field_278 = one;
            func_801307e0(obj, 0x1f);
        } else {
            func_80130dc0(obj);
        }
    }
}

void func_801b4b38_slot04_15(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_801b4b9c_slot04_15(Object *obj) {
    s16 t = obj->step_flags;
    s16 d;

    if (t >= 0) {
        if ((u8)t != 0) {
            d = -2;
            if (obj->field_0b != 0) {
                d = 2;
            }
            *(s32 *)&obj->pos_x_frac += d;
        }
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b4c10_slot04_15(Object *obj) {
    obj->field_157 = 1;
    data_801c3ce8_slot04_15[obj->field_12a >> 1](obj);
}

void func_801b4c58_slot04_15(Object *obj) {
    data_801c3cf4_slot04_15[obj->state_3](obj);
}

void func_801b4c98_slot04_15(Object *obj) {
    obj->field_159 = 1;
    obj->state_3++;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b4cd0_slot04_15(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b4d34_slot04_15(Object *obj) {
    u16 t;
    obj->state_3 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    t = 0x12;
    if (obj->field_48 != 0) {
        t = 0xc;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    t = (obj->field_12a >> 1) + t;
    func_801307e0(obj, t);
    func_80120af8(obj);
}
