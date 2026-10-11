/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c5b98_slot04_0f[];
extern ObjectFn data_801c5ba0_slot04_0f[];
extern ObjectFn data_801c5bb4_slot04_0f[];
extern ObjectFn data_801c5bbc_slot04_0f[];
extern ObjectFn data_801c5bc4_slot04_0f[];
extern ObjectFn data_801c5bd4_slot04_0f[];

void func_80130678(Object *object, int index);
void func_80131468(Object *object);
void func_80140598(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, int unused);
void func_80146960(Object *object);
void func_80146998(Object *object);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801b41dc_slot04_0f(Object *obj);
void func_801b45a8_slot04_0f(Object *obj);
void func_801b394c_slot04_0f(Object *obj);

void func_801b3b2c_slot04_0f(Object *obj) {
    u16 t;
    int m;
    int i;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        m = -0x100;
        obj->step_flags = t & m;
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 < 0) {
            obj->state_3 = obj->state_3 + 1;
            func_801307e0(obj, 0x53);
        } else {
            func_80146960(obj);
            i = *(u8 *)&obj->field_46 * 2;
            if (obj->field_12a == 4) {
                i += 2;
            }
            func_80140cd8(obj, (s16)(*(u16 *)((u8 *)data_801c5b98_slot04_0f + i) | m), 0);
            func_801204f4(obj, obj->side, 0xb);
            func_80120554(obj, obj->side, 0x319);
        }
    }
}

void func_801b3c14_slot04_0f(Object *obj) {
    Object *o;
    int one;

    obj->state_3 = obj->state_3 + 1;
    func_80146960(obj);
    one = 1;
    o = obj->other;
    o->field_15b = one;
    func_80140598(obj, 0, 5, 0x1f, 0, 0, 0);
    obj->vel_x = 0x24000;
    obj->vel_y = -0x60000;
    obj->accel_x = -0x500;
    obj->field_45 = one;
    obj->accel_y = 0x3000;
    if ((s16)o->field_5c < 0) {
        func_80120554(obj, obj->side, 0x319);
        obj->field_167 = (obj->field_12a >> 1) + 0xc;
        game_state.config->field_6b = 4;
        func_80147000(obj);
    } else {
        func_80120554(obj, obj->side, 0x319);
    }
}

void func_801b3d08_slot04_0f(Object *obj) {
    func_801b394c_slot04_0f(obj);
    if (obj->field_70 <= obj->pos_y) {
        obj->state_3 = obj->state_3 + 1;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        obj->pos_y_frac = 0;
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}

void func_801b3d84_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3dc4_slot04_0f(Object *obj) {
    data_801c5ba0_slot04_0f[obj->state_3](obj);
}

void func_801b3e04_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        func_80146998(obj);
    }
    update_sequence(obj);
}

void func_801b3e54_slot04_0f(Object *obj) {
    s32 a = 0xc0000;
    s32 b;

    if ((obj->step_flags << 16) < 0) {
        obj->state_3 = obj->state_3 + 1;
        b = -0x10000;
        obj->vel_y = 0;
        obj->accel_y = 0;
        if (obj->field_0b == 0) {
            a = -0xc0000;
            b = 0x10000;
        }
        obj->accel_x = b;
        obj->vel_x = a;
        func_801307e0(obj, 0x46);
    } else {
        update_sequence(obj);
    }
}

void func_801b3ec8_slot04_0f(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        func_80120554(obj, obj->side, 0x324);
    }
    update_sequence(obj);
}

void func_801b3f18_slot04_0f(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x == 0) {
        obj->state_3 = obj->state_3 + 1;
        func_801307e0(obj, 0x38);
    } else {
        update_sequence(obj);
    }
}

void func_801b3f80_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3fc0_slot04_0f(Object *obj) {
    data_801c5bb4_slot04_0f[obj->state_3](obj);
}

void func_801b4000_slot04_0f(Object *obj) {
    obj->field_17b = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    func_801307e0(obj, 0x63);
}

void func_801b4054_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4094_slot04_0f(Object *obj) {
    data_801c5bbc_slot04_0f[obj->state_3](obj);
}

void func_801b40d4_slot04_0f(Object *obj) {
    obj->field_17b = 1;
    obj->vel_x = 0;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->accel_y = -0x200;
    obj->state_3++;
    func_801b41dc_slot04_0f(obj);
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    ((Slot04bObj *)obj)->field_46 = 0x78;
    func_801307e0(obj, 0x64);
}

void func_801b415c_slot04_0f(Object *obj) {
    Slot04bObj *o = (Slot04bObj *)obj;

    func_801b41dc_slot04_0f(obj);
    o->field_46--;
    if (o->field_46 == 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 3;
        obj->state_3 = 1;
        obj->accel_y = -0x3000;
        func_80130678(obj, 0x21);
    } else {
        update_sequence(obj);
    }
}

void func_801b41dc_slot04_0f(Object *obj) {
    s16 t;

    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    t = obj->field_70;
    if (obj->pos_y >= t) {
        obj->pos_y = t;
        obj->pos_y_frac = 0;
    }
}

void func_801b4230_slot04_0f(Object *obj) {
    if (obj->field_12a != 2) {
        func_801b45a8_slot04_0f(obj);
    } else {
        data_801c5bc4_slot04_0f[obj->state_3](obj);
    }
}

void func_801b4290_slot04_0f(Object *obj) {
    if (obj->field_12a != 2) {
        func_801b45a8_slot04_0f(obj);
    } else {
        data_801c5bd4_slot04_0f[obj->state_3](obj);
    }
}

void func_801b42f0_slot04_0f(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    func_80120554(obj, obj->side ^ 1, 0x31a);
    obj->field_160 = 0xb4;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_336 = 0;
    func_801307e0(obj, 0x18);
}

void func_801b4358_slot04_0f(Object *obj) {
    Object *o;
    u16 t;
    int k;

    o = obj->other;
    ref_other.p = o;
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 1, -0x2e, 0x49);
        k = 0;
        if (((Slot04bObj *)obj)->field_336 == 0) {
            ((Slot04bObj *)obj)->field_336 = 0xff;
            k = 10;
        }
        if (func_80140cd8(obj, k, 0) != 0) {
            func_80140770(obj, 1, 5, -0x200, 0, 0, 0);
            func_80120554(o, o->side, 0x333);
            goto tail;
        }
        obj->field_46 = obj->field_46 + 1;
        func_80120554(o, o->side, 0x302);
    }
    func_80140fe0(obj);
    if ((s16)obj->field_46 != 0 && (o->field_15b == 0 || func_801410c8(obj) != 0)) {
        func_80146478(obj, 1, -0x2e, 0x49);
        func_80140770(obj, 1, 5, 0, 0, 0, 1);
tail:
        obj->state_3 = obj->state_3 + 1;
        func_80120554(o, o->side, 0x306);
        func_801307e0(obj, 0x54);
    } else {
        update_sequence(obj);
    }
}
