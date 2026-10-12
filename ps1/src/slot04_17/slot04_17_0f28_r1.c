/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
int func_80141b28(Object *object);
int func_80141e34(Object *object);
void func_80146998(Object *object);
void func_80142adc(Object *object);
void select_box_tables(Object *object);
void build_metrics(Object *object);

extern ObjectFnInt data_801ce260_slot04_17[];
extern ObjectFn data_801ce288_slot04_17[];
extern ObjectFn data_801ce2bc_slot04_17[];
extern u8 data_801ce2cc_slot04_17[];
extern s16 data_801ce2d0_slot04_17[];
extern ObjectFn data_801ce2d8_slot04_17[];
extern s32 data_801ce2f4_slot04_17[];
extern u8 data_801ce324_slot04_17[];
extern u8 data_801cdf50_slot04_17[];

void func_801b0fec_slot04_17(Object *obj);
void func_801b10d8_slot04_17(Object *obj);
int func_801b1324_slot04_17(Object *obj);
int func_801b1358_slot04_17(Object *obj);
void func_801b15c4_slot04_17(Object *obj);
void func_801b1800_slot04_17(Object *obj);
void func_801b1844_slot04_17(Object *obj);
void func_801b17b4_slot04_17(Object *obj);
void func_801b1c48_slot04_17(Object *obj);
void func_801b1f44_slot04_17(Object *obj);
void func_801b57f0_slot04_17(Object *obj);
int func_801b5724_slot04_17(Object *obj);
void func_801b5a34_slot04_17(Object *obj);

int func_801b0f28_slot04_17(Object *obj) {
    if (obj->field_7e != 0) goto c;
    if (obj->field_177 == 0) goto z;
c:
    if (func_801417cc(obj)) goto b;
z:
    return 0;
b:
    obj->state_0 = 1;
    obj->state_2 = 7;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 9;
    obj->field_0b = obj->field_158;
    return 1;
}

int func_801b0fb0_slot04_17(Object *obj) {
    if (func_80141b28(obj)) {
        func_801b0fec_slot04_17(obj);
        return 1;
    }
    return 0;
}

void func_801b0fec_slot04_17(Object *obj) {
    int a = 0x10;
    int b = 0x14;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xa;
    obj->field_159 = 1;
    obj->field_12a = 4;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x13;
        b = 0x17;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->field_27b = b;
    func_80146998(obj);
    func_801307e0(obj, 0x1e);
}

int func_801b109c_slot04_17(Object *obj) {
    if (func_80141b28(obj)) {
        func_801b10d8_slot04_17(obj);
        return 1;
    }
    return 0;
}

void func_801b10d8_slot04_17(Object *obj) {
    int a = 0x13;
    int b = 0x17;

    obj->state_0 = 1;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 0xb;
    obj->field_159 = 1;
    obj->field_12a = 4;
    obj->field_0b = obj->field_158;
    obj->field_157 = 0;
    obj->field_6b = 0;
    if (obj->kind != 0x11) {
        a = 0x18;
        b = 0x1c;
    }
    ref_other.p = obj->other;
    ref_other.p->field_6b = a;
    obj->field_27b = b;
    func_80146998(obj);
    func_801307e0(obj, 0x1f);
}

int func_801b1188_slot04_17(Object *obj) {
    if (obj->field_292 != 0) return 0;
    if ((s16)obj->field_c6 < 0x30) return 0;
    if (obj->field_45 == 0) {
        if (!func_80141e34(obj)) return 0;
        if (!(u8)func_80141788(obj)) return 0;
        obj->state_0 = 1;
        obj->state_2 = 8;
        obj->state_1 = 0;
        obj->state_3 = 0;
        obj->field_15a = 0xc;
        obj->field_0b = obj->field_158;
        return 1;
    }
    if (obj->field_7e != 0) return 0;
    if (!(u8)func_801418bc(obj)) return 0;
    obj->state_0 = 1;
    obj->state_2 = 8;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_15a = 0xc;
    return 1;
}

void func_801b127c_slot04_17(Object *obj) {
    u8 k;

    if (obj->field_15a < 8) {
        k = 0x11;
        if (obj->field_219 != 0) {
            k = 0x13;
        }
        if (obj->kind != k) {
            obj->kind = k;
            func_801b5a34_slot04_17(obj);
            select_box_tables(obj);
            build_metrics(obj);
        }
    }
    data_801ad398 = data_801ce260_slot04_17[obj->field_15a](obj);
}

int func_801b1324_slot04_17(Object *obj) {
    return obj->kind == 0x11;
}

int func_801b1338_slot04_17(Object *obj) {
    return func_801b1324_slot04_17(obj);
}

int func_801b1358_slot04_17(Object *obj) {
    return obj->kind == 0x13;
}

int func_801b136c_slot04_17(Object *obj) {
    return func_801b1358_slot04_17(obj);
}

int func_801b138c_slot04_17(Object *obj) {
    if (obj->kind != 0x11) return 0;
    return (s16)obj->field_c6 >= 0x30;
}

int func_801b13bc_slot04_17(Object *obj) {
    if (obj->kind == 0x11) {
        return (s16)obj->field_c6 >= 0x30;
    }
    return 0;
}

int func_801b13e4_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->field_c6 >= 0x30;
    }
    return 0;
}

int func_801b140c_slot04_17(Object *obj) {
    if (obj->kind == 0x13) {
        return (s16)obj->field_c6 >= 6;
    }
    return 0;
}

int func_801b1434_slot04_17(Object *obj) {
    return 1;
}

int func_801b143c_slot04_17(Object *obj) {
    return obj->field_177 != 0;
}

void func_801b1448_slot04_17(Object *obj) {
    data_801ce288_slot04_17[obj->field_15a](obj);
}

void func_801b1488_slot04_17(Object *obj) {
    data_801ce2bc_slot04_17[obj->state_3](obj);
}

void func_801b14c8_slot04_17(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_12d = 0xb4;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    obj->field_254 = 0;
    a = 0x21;
    if (obj->field_49 != 0) {
        a = 0x40;
    }
    a += obj->field_12a;
    func_801307e0(obj, a);
}

void func_801b1544_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a != 0) {
        o->field_46 = 0xf;
        o->state_3++;
        if (o->field_cd == 0) {
            func_801b15c4_slot04_17(o);
        } else {
            o->field_46 = data_801ce2cc_slot04_17[o->field_129];
            func_801b1844_slot04_17(o);
        }
    } else {
        update_sequence(o);
    }
}

void func_801b15c4_slot04_17(Object *obj) {
    s16 x;
    int h;
    u8 s;

    h = (s16)obj->step_flags >> 8;
    if (obj->field_cd != 0) {
        func_801b1844_slot04_17(obj);
    } else if ((*(u32 *)&game_state.field_4c & 0xffff00) != 0 || game_state.field_04 != 0 ||
               (func_8012f56c(obj) & 0xff) != 0) {
        func_801b1800_slot04_17(obj);
    } else {
        if ((h & 0x7f) != 0) {
            obj->step_flags = obj->step_flags & 0x80ff;
            func_80142c04(obj);
            func_80138ae8(&game_state, obj);
            if (obj->field_254 == 0) {
                obj->field_254 = 0xff;
            }
        }
        if ((obj->pressed_bits & 0x94) == 0) {
            func_801b17b4_slot04_17(obj);
        } else {
            if ((obj->pressed_bits & 0x80) != 0) {
                x = 0;
            } else if ((obj->pressed_bits & 0x10) != 0) {
                x = 2;
            } else {
                x = 4;
            }
            if ((s16)obj->field_46 <= data_801ce2d0_slot04_17[x >> 1]) {
                func_801b17b4_slot04_17(obj);
            } else {
                obj->field_46 = 0xf;
                if (obj->field_12a == x) {
                    update_sequence(obj);
                } else {
                    obj->field_12a = x;
                    s = ((Slot04bObj *)obj)->field_3a;
                    if (obj->field_49 != 0) {
                        x += 0x40;
                    } else {
                        x += 0x21;
                    }
                    func_801307e0(obj, x);
                    obj->sequence = obj->sequence + s;
                    obj->step_timer = obj->sequence->duration;
                    obj->frame = obj->frames + obj->sequence->frame_index;
                    obj->field_4a = obj->frame->field_0d;
                    obj->sequence_changed_flag = 1;
                }
            }
        }
    }
}

void func_801b17b4_slot04_17(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        update_sequence(obj);
    } else {
        func_801b1800_slot04_17(obj);
    }
}

void func_801b1800_slot04_17(Object *obj) {
    int a;

    obj->state_3++;
    obj->field_17b = 0;
    a = 0x22;
    if (obj->field_49 != 0) {
        a = 0x41;
    }
    a += obj->field_12a;
    func_801307e0(obj, a);
}

void func_801b1844_slot04_17(Object *obj) {
    u16 t;

    if ((game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) != 0 ||
        (func_8012f56c(obj) & 0xff) != 0) {
        goto tail;
    }
    t = obj->step_flags;
    if (((t >> 8) & 0x7f) != 0 && (t & 0x7f00) == 0x100) {
        obj->step_flags = t & 0x80ff;
        func_80142c04(obj);
        func_80138ae8(&game_state, obj);
        if (obj->field_254 == 0) {
            obj->field_254 = 0xff;
        }
        if (obj->field_129 != 0) {
            obj->field_46 = (s16)obj->field_46 - 1;
            if ((s16)obj->field_46 == 0) {
                goto tail;
            }
        } else if ((func_8014a170(obj, (u32 *)data_801cdf50_slot04_17) & 0xff) != 0) {
            goto tail;
        }
    }
    update_sequence(obj);
    return;
tail:
    obj->state_3++;
    obj->field_17b = 0;
    func_801307e0(obj, obj->field_12a + 0x22);
}

void func_801b1974_slot04_17(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b57f0_slot04_17(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}

void func_801b19e8_slot04_17(Object *obj) {
    data_801ce2d8_slot04_17[obj->state_3](obj);
}

void func_801b1a28_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;

    o->field_17b = 1;
    o->state_3++;
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    o->field_67 = 0;
    o->field_12f = 0;
    o->vel_x = data_801ce2f4_slot04_17[o->field_12a * 2];
    o->accel_x = data_801ce2f4_slot04_17[o->field_12a * 2 + 1];
    o->vel_y = data_801ce2f4_slot04_17[o->field_12a * 2 + 2];
    o->accel_y = data_801ce2f4_slot04_17[o->field_12a * 2 + 3];
    d = 0x10;
    if (o->field_0b == 0) {
        d = -0x10;
    }
    o->pos_x = o->pos_x + d;
    play_side_sound_at(o, obj->field_a6, 5);
    if (o->field_49 == 0) {
        func_801b1f44_slot04_17(o);
    } else {
        func_801307e0(o, (o->field_12a >> 1) + 0x46);
    }
}

void func_801b1b5c_slot04_17(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->state_3++;
        o->field_12e = data_801ce324_slot04_17[o->field_12a & 0xfe];
    }
    update_sequence(o);
}

void func_801b1bbc_slot04_17(Object *obj) {
    if (obj->field_67 != 0) {
        obj->vel_x = 0xc000;
        obj->vel_y = 0x19000;
        obj->accel_x = 0;
        obj->accel_y = -0x1000;
        obj->field_46 = 0x1801;
        obj->state_3++;
    }
    if (func_801b5724_slot04_17(obj) < 0) {
        func_801b1c48_slot04_17(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b1c48_slot04_17(Object *obj) {
    int a = -0x6000;

    obj->state_3 = 5;
    if (obj->field_49 != 0) {
        a = -0x12000;
    }
    obj->accel_y = a;
    func_801307e0(obj, 0x3c);
}
