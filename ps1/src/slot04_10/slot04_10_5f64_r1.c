/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s16 data_801c6ce0_slot04_10[];
extern ObjectFnInt data_801c6ce8_slot04_10[];
extern u8 data_801c6d18_slot04_10[];
extern u8 data_801c6d80_slot04_10[];
extern SequenceStep *data_801c6dbc_slot04_10[];
extern ObjectFn data_801c6dc8_slot04_10[];
extern ObjectFn data_801c6dd8_slot04_10[];
extern ObjectFn data_801c6e9c_slot04_10[];
extern ObjectRef data_80190468;

int func_80130184(Object *object);
u8 func_80140cd8(Object *object, int a, int b);
int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146960(Object *object);
void func_8011ffdc(Object *o);
void func_8011f14c(Slab172 *o);
void func_801b1078_slot04_10(Object *obj);
void func_801b1098_slot04_10(Object *obj);
void func_801b5dd0_slot04_10(Object *obj);

void func_801b5f64_slot04_10(Object *obj) {
    u16 t = obj->field_3a;

    if ((t & 0xff) == 0) {
        func_80130efc(obj);
    } else if ((t & 0xff) == 0xf) {
        obj->field_3a = t & 0xff00;
        data_80190468.p->field_63 = obj->field_12a * 4 + 0x3c;
        func_80146960(obj);
        func_80130efc(obj);
        func_801204f4(obj, obj->side, 6);
        if (func_80140cd8(obj, *(s16 *)((u8 *)data_801c6ce0_slot04_10 + (obj->field_12a & 0xfe)), 0) != 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        func_80120554(obj, obj->side, 0x319);
    } else {
        obj->field_45 = 1;
        obj->field_07 = obj->field_07 + 1;
        obj->field_3a = obj->field_3a & 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0x23, 0xf, -0x200, 0, 0, 1);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
        }
        obj->field_4c = 0x38000;
        obj->field_50 = 0x60000;
        obj->field_58 = -0x8000;
        obj->field_54 = 0;
        if (obj->field_0b == 0) {
            obj->field_4c = -obj->field_4c;
        }
    }
}

void func_801b6148_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07 = obj->field_07 + 1;
        obj->field_45 = 0;
        obj->pos_y = ((Slot04bObj *)obj)->field_70;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x2f);
    }
}

void func_801b61c4_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        obj->field_0b = obj->field_0b ^ 1;
        func_801b1098_slot04_10(obj);
    }
}

void func_801b6230_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801b1078_slot04_10(obj);
    }
}

void func_801b6270_slot04_10(Object *obj) {
    s16 t = obj->field_3a;

    if (t >= 0) {
        if ((t & 0xff) == 0xf) {
            func_80140cd8(obj, -0xed, 0);
            obj->field_3a = obj->field_3a & 0xff00;
            data_80190468.p->field_63 = obj->field_12a * 4 + 0x3c;
            func_80120554(obj, obj->side, 0x319);
            func_80146960(obj);
        }
        func_80130efc(obj);
    } else {
        obj->field_07 = 5;
        obj->field_45 = 1;
        ((Slot04bObj *)obj)->field_298 = 0;
        ((Slot04bObj *)obj)->field_1a8 = 2;
        func_801b5dd0_slot04_10(obj);
        func_801307e0(obj, 0x2c);
    }
}

void func_801b6338_slot04_10(Object *obj) {
    data_801ad398 = data_801c6ce8_slot04_10[obj->field_15a](obj);
}

int func_801b6380_slot04_10(Object *obj) {
    return obj->field_177 != 0;
}

int func_801b638c_slot04_10(Object *obj) {
    return *(s16 *)&obj->field_c6 >= 0x30;
}

int func_801b63a0_slot04_10(Object *obj) {
    return *(s16 *)&obj->field_c6 >= 0x30;
}

int func_801b63b4_slot04_10(Object *obj) {
    return 1;
}

int func_801b63bc_slot04_10(Object *obj) {
    return 1;
}

int func_801b63c4_slot04_10(Object *obj) {
    return 1;
}

int func_801b63cc_slot04_10(Object *obj) {
    return 1;
}

int func_801b63d4_slot04_10(Object *obj) {
    return 1;
}

int func_801b63dc_slot04_10(Object *obj) {
    return 1;
}

int func_801b63e4_slot04_10(Object *obj) {
    return 1;
}

int func_801b63ec_slot04_10(Object *obj) {
    return 1;
}

int func_801b63f4_slot04_10(Object *obj) {
    return 1;
}

void func_801b63fc_slot04_10(Object *obj) {
    s16 i;
    u16 j;

    for (i = 0; i < 7; i++) {
        ref_first.p = (Object *)((u8 *)obj + 0x2b0 + (i << 3));
        for (j = 0; j < 8; j++) {
            *(*(u8 **)&ref_first.p)++ = 0;
        }
    }
}

void func_801b6464_slot04_10(Object *obj) {
    data_801c6dc8_slot04_10[obj->field_04](obj);
}

void func_801b64a4_slot04_10(Object *obj) {
    u8 t;

    obj->field_a0 = 0xff;
    obj->field_04 = obj->field_04 + 1;
    obj->field_09 = 0;
    ref_other.p = obj->field_3c;
    t = ref_other.p->field_49;
    ((Slot04bObj *)obj)->field_6c = data_801c6d80_slot04_10;
    ((Slot04bObj *)obj)->field_8c = (s32)data_801c6d18_slot04_10;
    obj->field_45 = 0;
    ((Slot04bObj *)obj)->field_5c = 0;
    obj->field_49 = t;
    func_80130700(obj, data_801c6dbc_slot04_10[obj->field_ac >> 1]);
}

void func_801b6534_slot04_10(Object *obj) {
    data_801c6dd8_slot04_10[obj->field_05](obj);
    func_8011ffdc(obj);
}

void func_801b6588_slot04_10(Object *obj) {
    ref_other.p = obj->field_3c;
    if (ref_other.p->frame->active == 0) {
        obj->field_04 = obj->field_04 + 1;
    }
}

void func_801b65d4_slot04_10(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
}

void func_801b65e8_slot04_10(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801b6608_slot04_10(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c6e9c_slot04_10[obj->field_04](obj);
}

void func_801b6658_slot04_10(Object *obj) {
    obj->field_98 = ref_other.p->field_98;
    obj->field_9c = ref_other.p->field_9c;
    obj->field_04 = obj->field_04 + 1;
    obj->field_1c = ref_other.p->field_1c;
    obj->field_03 = ref_other.p->kind;
    obj->field_0c = ref_other.p->field_0c;
    obj->field_0e = ref_other.p->field_0e;
    obj->field_66 = ref_other.p->field_66;
    obj->field_48 = 0;
}
