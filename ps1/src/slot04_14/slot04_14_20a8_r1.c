/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c62d4_slot04_14[];
extern s32 data_801c62e0_slot04_14[];
extern u16 data_801c62ec_slot04_14[];
extern ObjectFn data_801c62f4_slot04_14[];
extern s32 data_801c6304_slot04_14[];
extern ObjectFn data_801c6310_slot04_14[];

void func_80138ae8(GameState *state, Object *object);
int func_80130184(Object *object);
void func_80130678(Object *object, int arg);
void func_801b2b4c_slot04_14(Object *obj);
void func_801b62a8_slot04_14(Object *obj, u8 a, u8 b, u8 c, u8 d);
u8 func_801b26bc_slot04_14(Object *obj);
void func_801b2664_slot04_14(Object *obj);

void func_801b20a8_slot04_14(Object *obj) {
    int i = 1;

    obj->field_17b = i;
    obj->field_45 = i;
    obj->state_3 = i;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    i = obj->field_12a >> 1;
    *(s32 *)&obj->vel_y = 0x40000;
    obj->accel_y = -0x6000;
    *(u32 *)&obj->pos_y_frac &= 0xffff0000;
    if (obj->field_49 != 0) {
        obj->accel_y = data_801c62d4_slot04_14[i];
    }
    obj->vel_x = -data_801c62e0_slot04_14[(i & 0xff)];
    if (obj->field_0b != 0) {
        obj->vel_x = data_801c62e0_slot04_14[(i & 0xff)];
    }
    func_801307e0(obj, data_801c62ec_slot04_14[(i & 0xff)]);
}

void func_801b2190_slot04_14(Object *obj) {
    u16 a;

    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y < 0) {
        obj->state_3++;
        play_sound_at(obj, obj->side, 0x320);
        if (obj->field_49 != 0) {
            a = 0x70;
        } else {
            a = 0x20;
        }
        ((Slot04bObj *)obj)->field_1a4 = 1;
        if (obj->field_12a != 0) {
            ((Slot04bObj *)obj)->field_1a4 = obj->field_12a;
        }
        a += obj->field_12a >> 1;
        func_801307e0(obj, a);
    } else {
        update_sequence(obj);
    }
}

void func_801b225c_slot04_14(Object *obj) {

    if ((s16)obj->step_flags & 0x8000) {
        play_sound_at(obj, obj->side, 0x320);
        ((Slot04bObj *)obj)->field_1a4--;
        if (((Slot04bObj *)obj)->field_1a4 == 0) {
            obj->state_3++;
            func_801307e0(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void func_801b22f4_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (func_801b26bc_slot04_14(obj) == 0) {
        func_801209c4(obj);
        s1->field_249 = 5;
        s1->pos_y_frac = 0;
        func_801312b8(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b2364_slot04_14(Object *obj) {
    data_801c62f4_slot04_14[obj->state_3](obj);
}

void func_801b23a4_slot04_14(Object *obj) {
    s32 d;

    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    if (obj->vel_y < 0) {
        d = obj->accel_y;
        obj->accel_y = d >> 1;
    } else {
        obj->accel_y = -0x4800;
    }
    d = data_801c6304_slot04_14[obj->field_12a >> 1];
    if (obj->field_0b != 0) {
        if (obj->vel_x < 0) {
            d = -d;
        }
    } else {
        if (obj->vel_x <= 0) {
            d = -d;
        }
    }
    obj->vel_x = d + obj->vel_x;
    play_sound_at(obj, obj->side, 0x320);
    ((Slot04bObj *)obj)->field_1a4 = 1;
    if (obj->field_12a != 0) {
        ((Slot04bObj *)obj)->field_1a4 = obj->field_12a;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + 0x23);
}

void func_801b24a8_slot04_14(Object *obj) {

    if (func_801b26bc_slot04_14(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            play_sound_at(obj, obj->side, 0x320);
            ((Slot04bObj *)obj)->field_1a4--;
            if (((Slot04bObj *)obj)->field_1a4 == 0) {
                obj->state_3++;
                func_801307e0(obj, 0x1f);
                return;
            }
        }
        update_sequence(obj);
    } else {
        func_801b2664_slot04_14(obj);
    }
}

void func_801b2550_slot04_14(Object *obj) {
    if (func_801b26bc_slot04_14(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_3++;
            func_80130678(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        func_801b2664_slot04_14(obj);
    }
}

void func_801b25cc_slot04_14(Object *obj) {
    Object *s1 = obj->other;

    if (func_801b26bc_slot04_14(obj) == 0) {
        func_801b62a8_slot04_14(obj, 1, 0, 3, 2);
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        func_801209c4(obj);
        s1->field_249 = 5;
        func_80130678(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}

void func_801b2664_slot04_14(Object *obj) {
    obj->state_3++;
    func_801209c4(obj);
    obj->field_159 = 0;
    obj->field_45 = 0;
    obj->pos_y_frac = 0;
    obj->pos_y = ((Slot04bObj *)obj)->field_70;
    func_80130678(obj, 0x11);
}

u8 func_801b26bc_slot04_14(Object *obj) {
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    return obj->field_70 > obj->pos_y;
}

void func_801b26fc_slot04_14(Object *obj) {
    data_801c6310_slot04_14[obj->state_3](obj);
}

void func_801b273c_slot04_14(Object *obj) {
    obj->state_3++;
    func_80141f28(obj, 3);
    func_80138ae8(&game_state, obj);
    func_801307e0(obj, 0x26);
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b2790_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 1) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0x1a;
                p->field_ad = 0;
                p->field_5c = 0;
                p->field_03 = 0;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_ac = obj->field_12a >> 1;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                ((Slot04bObj *)p)->field_70 = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_240++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b2904_slot04_14(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        func_801b2b4c_slot04_14(obj);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 2) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 0x14;
                p->field_03 = 2;
                p->field_ad = 0;
                p->field_5c = 0;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_ac = (obj->field_12a >> 1) + 3;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = ((Slot04bObj *)obj)->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                ((Slot04bObj *)p)->field_70 = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_240++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}
