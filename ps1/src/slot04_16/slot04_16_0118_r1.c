/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
extern u8 data_801b6e40_slot04_16[];
extern u8 data_801bb5bc_slot04_16[];
extern u32 data_801ca050_slot04_16[];
extern ObjectFn data_801ca0f0_slot04_16[];
extern ObjectFn data_801ca0f8_slot04_16[];
extern ObjectFn data_801ca100_slot04_16[];
extern ObjectFn data_801ca108_slot04_16[];
extern ObjectFn data_801ca110_slot04_16[];
Object *func_8011f32c(void);
u8 func_8013f8c4(Object *object, int a, int b);
int func_80141618(Object *object);
int func_801412a4(Object *object);
void func_801b0414_slot04_16(Object *obj);
void func_801b04ec_slot04_16(Object *obj);
void func_801b0334_slot04_16(Object *obj);
void func_801b0648_slot04_16(Object *obj);
void func_801b02f4_slot04_16(Object *obj);
void func_801b07e4_slot04_16(Object *obj);
void func_801b0688_slot04_16(Object *obj);
void func_801b090c_slot04_16(Object *obj);
void func_801b10b0_slot04_16(Object *obj);
extern ObjectFn data_801ca11c_slot04_16[];
void func_80130678(Object *object, int index);
void func_80130dc0(Object *object);
void func_80131468(Object *object);
int func_80130184(Object *object);
u8 func_80125734(Object *object, int a);
void func_801b0bdc_slot04_16(Object *obj);
void func_801b0c38_slot04_16(Object *obj);
void func_801b0c8c_slot04_16(Object *obj);
void func_801b0ce8_slot04_16(Object *obj);
void func_801b10f0_slot04_16(Object *obj);

void func_801b0118_slot04_16(Object *obj) {
    u32 *dst;
    u32 i;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b6e40_slot04_16;
    obj->field_9c = data_801bb5bc_slot04_16;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801ca050_slot04_16[i];
    }
    ref_other.p = func_8011f32c();
    if (ref_other.p != 0) {
        ref_other.p->field_00 = 1;
        ref_other.p->field_02 = 0x25;
        ref_other.p->field_3c = obj;
        ref_other.p->field_0c = obj->field_0c;
        ref_other.p->field_0e = obj->field_0e;
        obj->field_28 = (u32)ref_other.p;
        ref_other.p->field_66 = obj->field_66;
        ref_other.p->field_0d = obj->field_0d;
        ref_other.p->field_7a = obj->field_7a;
        ref_other.p->field_7c = obj->field_7c;
        ref_other.p->field_90 = obj->field_90;
        ref_other.p->field_98 = obj->field_98;
        ref_other.p->field_9c = obj->field_9c;
    }
}

void func_801b029c_slot04_16(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b090c_slot04_16(obj);
    } else if (obj->field_128 != 0) {
        func_801b0648_slot04_16(obj);
    } else {
        func_801b02f4_slot04_16(obj);
    }
}

void func_801b02f4_slot04_16(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 != 0) {
        func_801b04ec_slot04_16(obj);
    } else {
        func_801b0334_slot04_16(obj);
    }
}

void func_801b0334_slot04_16(Object *obj) {
    data_801ca0f0_slot04_16[obj->field_07](obj);
}

void func_801b0374_slot04_16(Object *obj) {
    int one;

    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) != 0 && func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_16(obj);
    } else {
        one = 1;
        obj->field_159 = one;
        func_80130dc0(obj);
        if (obj->field_12a == 4) {
            obj->field_278 = one;
        }
    }
}

void func_801b0414_slot04_16(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 2;
    obj->field_06 = 0;
    obj->field_07 = 0;
}

void func_801b0430_slot04_16(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_801b10b0_slot04_16(obj);
    } else {
        if ((t & 1) != 0) {
            s16 d = 3;
            if (obj->field_0b == 0) {
                d = -3;
            }
            obj->pos_x = d + obj->pos_x;
        }
        if ((obj->field_3a & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b04ec_slot04_16(Object *obj) {
    data_801ca0f8_slot04_16[obj->field_07](obj);
}

void func_801b052c_slot04_16(Object *obj) {
    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a != 0 && (obj->field_130 & 0xa000) == 0) {
        obj->field_159 = 1;
        func_80130dc0(obj);
        return;
    }
    if (obj->field_12a != 0 && func_8013f8c4(obj, -0x25, 0x14) != 0) {
        func_801b0414_slot04_16(obj);
        return;
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b05c4_slot04_16(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_801b10b0_slot04_16(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b0648_slot04_16(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b07e4_slot04_16(obj);
    } else {
        func_801b0688_slot04_16(obj);
    }
}

void func_801b0688_slot04_16(Object *obj) {
    data_801ca100_slot04_16[obj->field_07](obj);
}

void func_801b06c8_slot04_16(Object *obj) {
    obj->field_67 = 0;
    obj->field_07 = obj->field_07 + 1;
    if (obj->field_12a == 0) {
        obj->field_159 = 1;
        func_80130dc0(obj);
        return;
    }
    if ((obj->field_130 & 0x8000) != 0) {
        if (func_8013f8c4(obj, -0x25, 0x14) != 0) {
            func_801b0414_slot04_16(obj);
            return;
        }
    }
    obj->field_159 = 1;
    func_80130dc0(obj);
}

void func_801b0760_slot04_16(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b07e4_slot04_16(Object *obj) {
    data_801ca108_slot04_16[obj->field_07](obj);
}

void func_801b0824_slot04_16(Object *obj) {
    unsigned a;

    obj->field_159 = 1;
    obj->field_07 = obj->field_07 + 1;
    a = obj->field_12a;
    obj->field_67 = 0;
    if (a == 0 || (obj->field_130 & 0x2000) == 0) {
        func_80130dc0(obj);
    } else {
        func_801307e0(obj, (a >> 1) + 0x55);
    }
}

void func_801b0888_slot04_16(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if ((t & 0x80) != 0 && (u8)func_80141618(obj) != 0 || (u8)func_801412a4(obj) != 0) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}

void func_801b090c_slot04_16(Object *obj) {
    data_801ca110_slot04_16[obj->field_12f](obj);
}

void func_801b094c_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10f0_slot04_16(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b09c0_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10f0_slot04_16(obj);
        if (*(u32 *)&obj->field_04 == 0x2030001) {
            func_801307e0(obj, 0x43);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b0a50_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) == 0) {
        obj->pos_y = (u16)obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        obj->field_45 = 0;
        obj->field_159 = 0;
        func_801209c4(obj);
        func_801b10f0_slot04_16(obj);
        if (*(u32 *)&obj->field_04 == 0x2030001) {
            func_801307e0(obj, 0x44);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b0ae0_slot04_16(Object *obj) {
    u16 t;

    obj->field_07 = 3;
    obj->field_128 = 4;
    obj->field_159 = 1;
    func_80130504(obj);
    func_80141f28(obj, obj->field_12a >> 1);
    t = obj->field_130;
    if (obj->field_48 == 0) {
        if ((t & 0x1000) && obj->field_129 == 0 && obj->field_12a != 0) {
            func_801b0ce8_slot04_16(obj);
        } else {
            func_801b0bdc_slot04_16(obj);
        }
    } else if ((t & 0x4000) == 0) {
        func_801b0bdc_slot04_16(obj);
    } else if (obj->field_129 == 0) {
        if (obj->field_12a == 4) {
            func_801b0c38_slot04_16(obj);
        } else {
            func_801b0bdc_slot04_16(obj);
        }
    } else if (obj->field_12a == 4) {
        func_801b0bdc_slot04_16(obj);
    } else {
        func_801b0c8c_slot04_16(obj);
    }
}

void func_801b0bdc_slot04_16(Object *obj) {
    s16 a = 0xc;

    if (obj->field_48 != 0) {
        a = 0x12;
    }
    if (obj->field_129 != 0) {
        a += 3;
    }
    a += obj->field_12a >> 1;
    func_801307e0(obj, a);
}

void func_801b0c38_slot04_16(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 0;
    func_80141f28(obj, 1);
    func_801307e0(obj, 0x4c);
}

void func_801b0c8c_slot04_16(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 1;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x4d);
}

void func_801b0ce8_slot04_16(Object *obj) {
    obj->field_04 = 1;
    obj->field_05 = 0;
    obj->field_06 = 5;
    obj->field_07 = 0;
    obj->field_12f = 2;
    func_80141f28(obj, 1);
    func_801307e0(obj, (obj->field_12a >> 1) + 0x4e);
}

void func_801b0d48_slot04_16(Object *obj) {
    data_801ca11c_slot04_16[obj->field_06](obj);
}

void func_801b0d88_slot04_16(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b0dbc_slot04_16(Object *obj) {
    Config *c = game_state.config;
    if (c->field_64 == 0 && c->field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}

void func_801b0e14_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 tbl[16] = { 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
    int v;

    o->field_06 = o->field_06 + 1;
    o->field_46 = 0x50;
    if (game_state.config->field_76 == 0) {
        game_state.config->field_76 = 0x1e;
    }
    v = (u8)func_80125734(o, tbl[func_80151184() & 0xf]);
    obj->field_330 = v;
    func_80130678(o, v + 0x23);
}
