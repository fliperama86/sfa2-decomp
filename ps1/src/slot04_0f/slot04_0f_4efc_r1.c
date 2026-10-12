/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c5e6c_slot04_0f[];
extern ObjectFn data_801c5e74_slot04_0f[];
extern s32 data_801c5e7c_slot04_0f[];
extern s32 data_801c5e88_slot04_0f[];
extern ObjectFn data_801c5e94_slot04_0f[];
extern u8 data_801c605c_slot04_0f[];
extern ObjectFn data_801c6064_slot04_0f[];
extern ObjectFn data_801c6074_slot04_0f[];
extern ObjectFn data_801c6088_slot04_0f[];
extern ObjectFn data_801c6294_slot04_0f[];
extern ObjectRef data_80190458;

void func_801b52b4_slot04_0f(Object *obj);
void func_801b5678_slot04_0f(Object *obj);
void func_801b57b4_slot04_0f(Object *obj);
void func_801b5830_slot04_0f(Object *obj, int a);
void func_801b54e8_slot04_0f(Object *obj);
void func_80137be0(Object *obj);
void func_80137cc0(Object *obj);
void func_80137dc8(Object *obj);
void func_80137f00(Object *obj);
void func_8011f14c(Slab172 *o);
void func_8011ffdc(Object *o);

void func_801b4efc_slot04_0f(Object *obj) {
    Config *config = game_state.config;
    u16 t;

    if ((config->field_65 | config->field_a8) == 0) {
        advance_sequence_checked(obj);
        t = obj->step_flags;
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            obj->vel_x >>= 1;
            obj->vel_y >>= 1;
        }
        if ((s16)obj->step_flags < 0) {
            obj->state_0 = 2;
            obj->state_1 = 0;
            obj->state_2 = 0;
            obj->state_3 = 0;
            func_801b52b4_slot04_0f(obj);
            return;
        }
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        *(s32 *)&obj->pos_y_frac -= obj->vel_y;
        func_8011ff74(obj);
    }
    func_8011ffdc(obj);
}

void func_801b4fd0_slot04_0f(Object *obj) {
    func_80137be0(obj);
}

void func_801b4ff0_slot04_0f(Object *obj) {
    func_80137cc0(obj);
}

void func_801b5010_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        data_801c5e6c_slot04_0f[obj->state_2](obj);
    }
    func_8011ffdc(obj);
}

void func_801b5080_slot04_0f(Object *o) {
    o->state_2++;
    o->field_0b ^= 1;
    func_801380f0(o);
    o->field_46 = 2;
    advance_sequence_checked(o);
}

void func_801b50d0_slot04_0f(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 == 0) {
        obj->field_00 = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->vel_y = 0;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
        func_80138070(obj, obj->field_a0);
    }
    advance_sequence_checked(obj);
}

void func_801b5150_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        data_801c5e74_slot04_0f[obj->state_2](obj);
    }
    func_8011ffdc(obj);
}

void func_801b51c0_slot04_0f(Object *o) {
    o->state_2++;
    o->field_0b ^= 1;
    func_801380f0(o);
    o->field_46 = 2;
    advance_sequence_checked(o);
}

void func_801b5210_slot04_0f(Object *obj) {
    int x;
    u8 i;
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    s32 unused[2];

    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 < 0) {
        obj->field_00 = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        i = obj->field_ac >> 1;
        x = data_801c5e7c_slot04_0f[i];
        obj->vel_y = data_801c5e88_slot04_0f[i];
        if (obj->vel_x >= 0) {
            x = -x;
        }
        obj->vel_x = x;
        func_80138070(obj, obj->field_a0);
    } else {
        advance_sequence_checked(obj);
    }
}

void func_801b52b4_slot04_0f(Object *obj) {
    data_801c5e94_slot04_0f[obj->state_1](obj);
    func_8011ffdc(obj);
}

void func_801b5308_slot04_0f(Object *obj) {
    obj->state_1++;
    obj->field_3c->field_14c = 0;
    func_80138070(obj, 0xb);
}

void func_801b533c_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void func_801b537c_slot04_0f(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b53c8_slot04_0f(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c6064_slot04_0f[obj->state_0](obj);
}

void func_801b5418_slot04_0f(Object *obj) {
    obj->ot_depth = 0;
    obj->state_0++;
    obj->field_49 = ref_other.p->field_49;
    obj->field_1c = ref_other.p->field_1c;
    ((Slot04bObj *)obj)->field_6c = data_801c605c_slot04_0f;
    obj->field_45 = 0;
    func_801b57b4_slot04_0f(obj);
    obj->field_a0 = 0xff;
    if (ref_other.p->side == 0) {
        ((Slot04bObj *)obj)->field_8c = *(s32 *)0x1f8000a8;
    } else {
        ((Slot04bObj *)obj)->field_8c = *(s32 *)0x1f800158;
    }
    func_801b5830_slot04_0f(obj, obj->field_ac >> 1);
    func_801b54e8_slot04_0f(obj);
}

void func_801b54e8_slot04_0f(Object *obj) {
    data_801c6074_slot04_0f[obj->state_1](obj);
}

void func_801b5528_slot04_0f(Object *obj) {
    Config *config = game_state.config;

    if ((config->field_65 | config->field_a8) == 0) {
        ref_other.p = obj->field_3c;
        if (*(u16 *)&ref_other.p->state_0 == 1 && ref_other.p->state_2 == 7) {
            advance_sequence_checked(obj);
            func_801b57b4_slot04_0f(obj);
            if ((s16)obj->step_flags >= 0) {
                goto go;
            }
        }
        obj->state_0 = 2;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        func_801b5678_slot04_0f(obj);
        return;
go:
        func_8011ff74(obj);
    }
    func_8011ffdc(obj);
}

void func_801b55f8_slot04_0f(Object *obj) {
    func_80137be0(obj);
}

void func_801b5618_slot04_0f(Object *obj) {
    func_80137cc0(obj);
}

void func_801b5638_slot04_0f(Object *obj) {
    func_80137dc8(obj);
}

void func_801b5658_slot04_0f(Object *obj) {
    func_80137f00(obj);
}

void func_801b5678_slot04_0f(Object *obj) {
    data_801c6088_slot04_0f[obj->state_1](obj);
    func_8011ffdc(obj);
}

void func_801b56cc_slot04_0f(Object *obj) {
    int n = 6;

    obj->state_1++;
    if (obj->field_03 != 0) {
        n = 7;
    }
    func_801b5830_slot04_0f(obj, n);
}

void func_801b5708_slot04_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0 = 3;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
    }
    advance_sequence_checked(obj);
}

void func_801b5748_slot04_0f(Object *o) {
    ref_other.p = o->field_3c;
    ref_other.p->field_240--;
    if (ref_other.p->field_240 == 0) {
        ref_other.p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b57b4_slot04_0f(Object *obj) {
    int a;
    int b;

    ref_other.p = obj->field_3c;
    *(s32 *)&obj->pos_x_frac = *(s32 *)&ref_other.p->pos_x_frac;
    a = -0x6c;
    b = -0x40;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&ref_other.p->pos_y_frac;
    if (obj->field_03 != 0) {
        a = -0x4b;
        b = -0x80;
    }
    if (obj->field_0b != 0) {
        a = -a;
    }
    obj->pos_x = a + obj->pos_x;
    obj->pos_y = b + obj->pos_y;
}

void func_801b5830_slot04_0f(Object *obj, int a) {
    if (obj->field_49 != 0) {
        a += 0x12;
    }
    func_80138070(obj, a);
}

void func_801b5864_slot04_0f(Object *obj) {
    data_80190458.p = obj->field_3c;
    data_801c6294_slot04_0f[obj->state_0](obj);
}
