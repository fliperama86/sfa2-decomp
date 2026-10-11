/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6560_slot04_14[];
extern ObjectFn data_801c6570_slot04_14[];
extern ObjectFn data_801c6578_slot04_14[];
extern u8 data_801c660c_slot04_14[];
extern ObjectFn data_801c6614_slot04_14[];
extern s32 data_801c6630_slot04_14[];
extern ObjectFn data_801c663c_slot04_14[];

void func_80130dc0(Object *obj);
void func_801b6840_slot04_14(Object *obj);
void func_801b6a10_slot04_14(Object *obj);
void func_801b6bcc_slot04_14(Object *obj);
void func_801b6c3c_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b6db4_slot04_14(Object *obj);
void func_801b6e50_slot04_14(Object *o);
void func_801b72b8_slot04_14(Object *obj, u8 a);

void func_801b6464_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        if (obj->field_128 == 2) {
            func_80131468(obj);
        } else {
            func_801312b8(obj);
        }
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b64ec_slot04_14(Object *obj) {
    s16 t = obj->step_flags;
    u8 a;

    if (t & 0x8000) {
        func_801312b8(obj);
        return;
    }
    a = t;
    if (a != 0) {
        if (a == 2) {
            obj->step_flags = (t & -0x100) | 1;
        }
        if (obj->field_0b != 0) {
            *(s32 *)&obj->pos_x_frac += 0x20000;
        } else {
            *(s32 *)&obj->pos_x_frac -= 0x20000;
        }
    }
    func_80130efc(obj);
}

void func_801b657c_slot04_14(Object *obj) {
    data_801c6560_slot04_14[obj->state_3](obj);
}

void func_801b65bc_slot04_14(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 0 && obj->field_218 != 0) {
        if (func_8013f8c4(obj, -0x14, 0x14) & 0xff) {
            func_801b6c3c_slot04_14(obj, 1, 2, 0, 0);
            return;
        }
        if (obj->field_12a == 2 && obj->field_219 != 0) {
            obj->field_159 = 1;
            obj->state_3 = 2;
            obj->field_157 = 0;
            func_80141f28(obj, 1);
            if (obj->field_0b != 0) {
                obj->vel_x = 0x36000;
            } else {
                obj->vel_x = -0x36000;
            }
            obj->vel_y = 0x40000;
            obj->accel_y = -0x6000;
            func_801307e0(obj, 0x40);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b66d4_slot04_14(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        obj->vel_y += obj->accel_y;
        if (obj->pos_y >= obj->field_70) {
            obj->state_3 = obj->state_3 + 1;
            obj->field_159 = 0;
            obj->field_45 = 0;
            obj->pos_y = ((Slot04bObj *)obj)->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x41);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b678c_slot04_14(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b67d0_slot04_14(Object *obj) {
    data_801c6570_slot04_14[obj->state_3](obj);
}

void func_801b6810_slot04_14(Object *obj) {
    obj->field_159 = 1;
    obj->state_3 = 1;
    obj->field_0b = obj->field_158;
    func_80130dc0(obj);
}

void func_801b6840_slot04_14(Object *obj) {
    int d;

    if (obj->field_45 != 0) {
        if (obj->field_218 != 0) {
            if ((s16)obj->field_c6 >= 0x30 && obj->field_14c == 0) {
                func_801b6c3c_slot04_14(obj, 1, 0, 8, 0);
                obj->field_15a = 9;
                obj->field_159 = 1;
                if (obj->side != 0) {
                    scratch_call_right(obj);
                } else {
                    scratch_call_left(obj);
                }
                return;
            }
        } else if (obj->field_129 != 0) {
            func_801b6c3c_slot04_14(obj, 1, 0, 7, 0);
            obj->field_15a = 3;
            obj->field_159 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        } else if (obj->field_14c == 0 && (d = ((Slot04bObj *)obj)->field_70 - 0x30, obj->pos_y < (s16)d)) {
            func_801b6c3c_slot04_14(obj, 1, 0, 7, 0);
            obj->field_15a = 4;
            obj->field_159 = 1;
            if (obj->side != 0) {
                scratch_call_right(obj);
            } else {
                scratch_call_left(obj);
            }
            return;
        }
    }
    obj->field_211 = (obj->field_211 & 0xfe) | 0x40;
    func_801b6a10_slot04_14(obj);
}

void func_801b69cc_slot04_14(Object *obj) {
    if (obj->field_211 & 1) {
        func_801b6840_slot04_14(obj);
    } else {
        func_801b6a10_slot04_14(obj);
    }
}

void func_801b6a10_slot04_14(Object *obj) {
    int t;

    obj->state_3 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80120af8(obj);
    t = obj->field_12a >> 1;
    func_80141f28(obj, t);
    if (obj->field_48 != 0) {
        t += 0x12;
    } else {
        t += 0xc;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    func_801307e0(obj, (s16)t);
    func_80120af8(obj);
    if (obj->pos_y < (u16)(((Slot04bObj *)obj)->field_70 - 0x50) && obj->field_48 != 0 && (obj->field_48 & 0x80) == 0 && obj->field_129 != 0 && obj->field_12a == 2 && obj->field_219 != 0) {
        func_801b6c3c_slot04_14(obj, 1, 0, 5, 0);
        func_801307e0(obj, 0x29);
    }
}

void func_801b6b40_slot04_14(Object *obj) {
    data_801c6578_slot04_14[obj->state_3](obj);
}

void func_801b6b80_slot04_14(Object *obj) {
    obj->vel_y = -0x4c000;
    obj->state_3 = obj->state_3 + 1;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x40000;
    } else {
        obj->vel_x = -0x40000;
    }
    func_801b6bcc_slot04_14(obj);
}

void func_801b6bcc_slot04_14(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    t = obj->field_70;
    if (t <= obj->pos_y) {
        obj->pos_y = t;
        func_801312b8(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b6c3c_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->state_0 = a;
    obj->state_1 = b;
    obj->state_2 = c;
    obj->state_3 = d;
}

void func_801b6c54_slot04_14(Object *obj) {
    data_801c6614_slot04_14[obj->state_0](obj);
}

void func_801b6c94_slot04_14(Object *obj) {
    Object *p;

    ((Slot04bObj *)obj)->field_6c = data_801c660c_slot04_14;
    obj->state_0 = obj->state_0 + 1;
    p = obj->field_3c;
    obj->field_49 = p->field_49;
    obj->field_1c = p->field_1c;
    obj->pos_y = obj->pos_y - 0x31;
    obj->vel_y = data_801c6630_slot04_14[obj->field_ac];
    if (obj->field_0b != 0) {
        obj->pos_x = obj->pos_x + 0x3e;
        obj->vel_x = data_801c6630_slot04_14[obj->field_ac - 3];
    } else {
        obj->pos_x = obj->pos_x - 0x3e;
        obj->vel_x = -data_801c6630_slot04_14[obj->field_ac - 3];
    }
    if (p->side == 0) {
        ((Slot04bObj *)obj)->field_8c = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->field_8c = *(s32 *)0x1f800158;
    }
    func_801b72b8_slot04_14(obj, obj->field_ac);
    func_801b6db4_slot04_14(obj);
}

void func_801b6db4_slot04_14(Object *obj) {
    data_801c663c_slot04_14[obj->state_1](obj);
}

void func_801b6df4_slot04_14(Object *obj) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        func_801b6e50_slot04_14(obj);
        func_80131094(obj);
        func_8011ff74(obj);
    }
    func_8011ffdc(obj);
}
