/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c73fc_slot04_11[];
extern ObjectFn data_801c7404_slot04_11[];
extern s32 data_801c7414_slot04_11[];
extern ObjectFn data_801c7420_slot04_11[];
extern u8 data_801c7444_slot04_11[];
extern u8 data_801c744c_slot04_11[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);
int func_801b57a8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);
void func_801b5874_slot04_11(Object *obj);
void func_801b5894_slot04_11(Object *obj);
Object *func_801b5914_slot04_11(Object *obj);

void func_801b1e54_slot04_11(Object *obj) {
    func_801b57a8_slot04_11(obj);
    if (obj->pos_y >= obj->field_70) {
        obj->state_3++;
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x3d);
    } else {
        update_sequence(obj);
    }
}

void func_801b1ee0_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b1f54_slot04_11(Object *obj) {
    u16 t = *(u16 *)(data_801c73fc_slot04_11 + (obj->field_12a & 0xfe));

    t = obj->field_12f + t;
    func_801307e0(obj, t);
}

void func_801b1f9c_slot04_11(Object *obj) {
    data_801c7404_slot04_11[obj->state_3](obj);
}

void func_801b1fdc_slot04_11(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_29c = 2;
    obj->state_3++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->field_0b = obj->field_158;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x3a;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}

void func_801b2060_slot04_11(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->vel_x = data_801c7414_slot04_11[obj->field_12a >> 1];
    }
    update_sequence(obj);
}

void func_801b20c0_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s32 d = o->vel_x;

    if (o->field_0b == 0) {
        d = -d;
    }
    *(s32 *)&o->pos_x_frac += d;
    if (*(u8 *)&o->step_flags == 0) {
        o->accel_x = -0xc000;
        obj->field_47 = 3;
        o->field_29c = 0;
        o->state_3++;
    }
    update_sequence(o);
}

void func_801b212c_slot04_11(Object *o) {
    s32 d;
    u16 t;

    if (((Slot04bObj *)o)->field_47 != 0) {
        ((Slot04bObj *)o)->field_47--;
        if (((Slot04bObj *)o)->field_47 == 0) {
            ((Slot04bObj *)o)->field_47 = 3;
            func_80148e84(o);
        }
    }
    if ((o->step_flags << 16) < 0) {
        ref_other.p = o->other;
        ref_other.p->field_249 = 5;
        o->field_17b = 0;
        func_801b5894_slot04_11(o);
    } else {
        d = o->vel_x;
        if (o->field_0b == 0) {
            d = -d;
        }
        *(s32 *)&o->pos_x_frac += d;
        o->vel_x += o->accel_x;
        if (o->vel_x < 0) {
            o->vel_x = 0;
            o->accel_x = 0;
            ((Slot04bObj *)o)->field_47 = 0;
        }
        t = o->step_flags;
        if ((t & 0xff) != 0) {
            o->step_flags = t & 0xff00;
            o->field_17b = 0;
        }
        func_80142adc(o);
        update_sequence(o);
    }
}

void func_801b2238_slot04_11(Object *obj) {
    data_801c7420_slot04_11[obj->state_3](obj);
}

void func_801b2278_slot04_11(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 6);
    func_80138ae8(&game_state, obj);
    a = 0x24;
    if (obj->field_49 != 0) {
        a = 0x3d;
    }
    func_801307e0(obj, a);
}

void func_801b22e4_slot04_11(Object *obj) {
    u16 t;
    s32 d;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags == 0) {
        t = obj->field_130;
        obj->field_45 = 1;
        obj->state_3++;
        obj->field_48 = 0;
        obj->field_67 = 0;
        if (obj->field_cd != 0) {
            t = func_80151184();
        }
        if (t & 0x8000) {
            obj->field_0b ^= 1;
        }
        d = *(s32 *)&func_801b5914_slot04_11(obj)->pos_x_frac;
        if (obj->field_0b == 0) {
            d += 0x1800000;
        }
        d -= *(s32 *)&obj->pos_x_frac;
        d /= *(u16 *)(data_801c7444_slot04_11 + (obj->field_12a & 0xfe));
        if (obj->field_49 != 0) {
            d <<= 2;
        }
        obj->vel_y = 0xa5000;
        obj->vel_x = d;
        obj->accel_x = 0;
        obj->accel_y = -0x8000;
    }
}

void func_801b23f8_slot04_11(Object *obj) {
    int r = func_801b5764_slot04_11(obj);

    if (r < 0 && obj->pos_y >= obj->field_70) {
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->state_3 = 8;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801307e0(obj, 0x37);
    } else if (obj->pos_y < obj->field_70 - 0x48 && (data_801c744c_slot04_11[obj->field_0b] & obj->field_164)) {
        obj->state_3++;
        func_801209c4(obj);
        obj->vel_y = 0;
        obj->accel_y = 0;
        func_801307e0(obj, 0x25);
    } else {
        update_sequence(obj);
    }
}
