/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801cdfd0_slot04_17[];
extern u8 data_801ce32c_slot04_17[];
extern u8 data_801ce330_slot04_17[];
extern ObjectFn data_801ce338_slot04_17[];
extern s32 data_801ce348_slot04_17[];
extern ObjectFn data_801ce354_slot04_17[];
extern u8 data_801ce378_slot04_17[];
extern u8 data_801ce380_slot04_17[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1c48_slot04_17(Object *obj);
void func_801b1f44_slot04_17(Object *obj);
int func_801b5724_slot04_17(Object *obj);
int func_801b56e0_slot04_17(Object *obj);
void func_801b57f0_slot04_17(Object *obj);
void func_801b5810_slot04_17(Object *obj);
Object *func_801b5890_slot04_17(Object *obj);

void func_801b1c88_slot04_17(Object *o) {
    u8 t;

    if (func_801b5724_slot04_17(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        func_801b1c48_slot04_17(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->field_46++;
        if (o->field_67 != 0) {
            if (o->field_cd != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (func_8014a170(o, (u32 *)data_801cdfd0_slot04_17) & 0xff) == 0)
                                 : ((o->pressed_bits & data_801ce32c_slot04_17[o->field_12a >> 1]) != 0)) {
                o->field_12f++;
                t = ((Slot04bObj *)o)->field_46;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                play_side_sound_at(o, o->side, 6);
                ((Slot04bObj *)o)->field_46 = 0;
                o->field_67 = 0;
                if (o->field_12f == o->field_12e) {
                    o->accel_x = -0x2000;
                    o->accel_y = -0x5000;
                    o->state_3++;
                }
                o->vel_x = 0xc000;
                o->vel_y = 0x19000;
                func_801b1f44_slot04_17(o);
            }
        }
        update_sequence(o);
    }
}

void func_801b1dec_slot04_17(Object *obj) {
    int a;

    if (func_801b5724_slot04_17(obj) < 0) {
        a = -0x7000;
        obj->state_3++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->accel_y = a;
    }
    update_sequence(obj);
}

void func_801b1e44_slot04_17(Object *obj) {
    func_801b5724_slot04_17(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->state_3++;
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x3d);
    } else {
        update_sequence(obj);
    }
}

void func_801b1ed0_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b1f44_slot04_17(Object *obj) {
    u16 t = *(u16 *)(data_801ce330_slot04_17 + (obj->field_12a & 0xfe));

    t = obj->field_12f + t;
    func_801307e0(obj, t);
}

void func_801b1f8c_slot04_17(Object *obj) {
    data_801ce338_slot04_17[obj->state_3](obj);
}

void func_801b1fcc_slot04_17(Object *obj) {
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

void func_801b2050_slot04_17(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->vel_x = data_801ce348_slot04_17[obj->field_12a >> 1];
    }
    update_sequence(obj);
}

void func_801b20b0_slot04_17(Object *o) {
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

void func_801b211c_slot04_17(Object *o) {
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
        func_801b5810_slot04_17(o);
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

void func_801b2228_slot04_17(Object *obj) {
    data_801ce354_slot04_17[obj->state_3](obj);
}

void func_801b2268_slot04_17(Object *obj) {
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

void func_801b22d4_slot04_17(Object *obj) {
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
            t = get_random_byte();
        }
        if (t & 0x8000) {
            obj->field_0b ^= 1;
        }
        d = *(s32 *)&func_801b5890_slot04_17(obj)->pos_x_frac;
        if (obj->field_0b == 0) {
            d += 0x1800000;
        }
        d -= *(s32 *)&obj->pos_x_frac;
        d /= *(u16 *)(data_801ce378_slot04_17 + (obj->field_12a & 0xfe));
        if (obj->field_49 != 0) {
            d <<= 2;
        }
        obj->vel_y = 0xa5000;
        obj->vel_x = d;
        obj->accel_x = 0;
        obj->accel_y = -0x8000;
    }
}

void func_801b23e8_slot04_17(Object *obj) {
    int r = func_801b56e0_slot04_17(obj);

    if (r < 0 && obj->pos_y >= obj->ground_y) {
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->state_3 = 8;
        obj->field_17b = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801307e0(obj, 0x37);
    } else if (obj->pos_y < obj->ground_y - 0x48 && (data_801ce380_slot04_17[obj->field_0b] & obj->field_164)) {
        obj->state_3++;
        func_801209c4(obj);
        obj->vel_y = 0;
        obj->accel_y = 0;
        func_801307e0(obj, 0x25);
    } else {
        update_sequence(obj);
    }
}
