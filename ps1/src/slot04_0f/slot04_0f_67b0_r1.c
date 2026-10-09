/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c63b0_slot04_0f[];
extern ObjectFn data_801c63c0_slot04_0f[];
extern ObjectFn data_801c63cc_slot04_0f[];
extern SequenceStep **data_1f8000b0;
extern SequenceStep **data_1f800160;

void func_8011ffdc(Object *o);
void func_8011f240(Slab172 *s);
void func_801b6818_slot04_0f(Object *obj);
void func_801b68d8_slot04_0f(Object *obj);
void func_801b6cf4_slot04_0f(Object *obj);
void func_801b6e78_slot04_0f(Object *obj);

void func_801b67b0_slot04_0f(Object *obj) {
    obj->field_07 = 3;
    obj->field_159 = 1;
    obj->field_128 = 4;
    func_80120af8(obj);
    if (obj->field_129 != 0) {
        func_801b68d8_slot04_0f(obj);
    } else {
        func_801b6818_slot04_0f(obj);
    }
}

void func_801b6818_slot04_0f(Object *obj) {
    int t;

    if (obj->field_12a != 0) {
        func_80141f28(obj, 1);
    }
    t = 0xc;
    if (obj->field_48 != 0) {
        t = 0x12;
    }
    if (obj->field_219 == 0) {
        if (obj->field_218 != 0 && obj->pos_y < obj->field_70 - 0x20) {
            obj->field_04 = 1;
            obj->field_05 = 0;
            obj->field_06 = 5;
            obj->field_07 = 0;
            t = 0x32;
            goto call;
        }
    } else {
        t += 0x1a;
    }
    t += obj->field_12a >> 1;
call:
    func_801307e0(obj, t);
}

void func_801b68d8_slot04_0f(Object *obj) {
    int t;

    if (obj->field_12a != 0) {
        func_80141f28(obj, 1);
    }
    t = 0xf;
    if (obj->field_48 != 0) {
        t = 0x15;
    }
    if (obj->field_219 != 0) {
        t += 0x1a;
    } else if (obj->field_218 != 0 && obj->pos_y < obj->field_70 - 0x20) {
        obj->field_04 = 1;
        obj->field_05 = 0;
        obj->field_06 = 5;
        obj->field_07 = 0;
        t = 0x33;
    }
    func_801307e0(obj, t + (obj->field_12a >> 1));
}

void func_801b6994_slot04_0f(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c63b0_slot04_0f[obj->field_04](obj);
}

void func_801b69e4_slot04_0f(Object *obj) {
    SequenceStep **t;

    obj->field_04++;
    if (ref_other.p->side == 0) {
        t = data_1f8000b0;
    } else {
        t = data_1f800160;
    }
    func_80130768(obj, 0, t);
}

void func_801b6a48_slot04_0f(Object *obj) {
    data_801c63c0_slot04_0f[obj->field_05](obj);
}

void func_801b6a88_slot04_0f(Object *obj) {
    ref_other.p = obj->field_3c;
    if ((u8)ref_other.p->field_3a != 0) {
        obj->field_05 = obj->field_05 + 1;
        func_8011ffdc(obj);
    }
}

void func_801b6ae0_slot04_0f(Object *obj) {
    int a;

    func_80131094(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_46 = 0x1a;
        obj->field_05 = obj->field_05 + 1;
        a = 0x8000;
        if (obj->field_0b != 0) {
            a = -0x8000;
        }
        obj->field_4c = a;
        obj->field_50 = 0;
        obj->field_58 = 0x1000;
    }
    func_8011ffdc(obj);
}

void func_801b6b50_slot04_0f(Object *obj) {
    s16 t;

    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
    obj->field_50 += obj->field_58;
    obj->field_46 = (s16)obj->field_46 - 1;
    t = obj->field_46;
    if (t == 0) {
        obj->field_04 = obj->field_04 + 1;
    } else {
        if (t < 0x12 && (t & 1) == 0) {
            obj->field_01 = 0;
        } else {
            obj->field_01 = 1;
            func_8011ffdc(obj);
        }
    }
}

void func_801b6bf4_slot04_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b6c14_slot04_0f(Object *obj) {
}

void func_801b6c1c_slot04_0f(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801c63cc_slot04_0f[obj->field_04](obj);
}

void func_801b6c6c_slot04_0f(Object *obj) {
    obj->field_46 = 0x20;
    obj->field_44 = 1;
    obj->field_04 = obj->field_04 + 1;
    if (ref_other.p->side == 0) {
        func_80130768(obj, 0xc, seqs_a4_left);
    } else {
        func_80130768(obj, 0xc, seqs_154_right);
    }
    func_801b6cf4_slot04_0f(obj);
}

void func_801b6cf4_slot04_0f(Object *obj) {
    if (game_state.config->field_65 == 0 && ref_other.p->field_6b == 0) {
        obj->field_46 = (s16)obj->field_46 - 1;
        if ((s16)obj->field_46 == 0) {
            obj->field_04 = obj->field_04 + 1;
            if (ref_other.p->side == 0) {
                func_80130768(obj, 0x11, seqs_a4_left);
            } else {
                func_80130768(obj, 0x11, seqs_154_right);
            }
        }
        func_801b6e78_slot04_0f(obj);
        func_80131094(obj);
    }
    func_8011ffdc(obj);
}

void func_801b6dcc_slot04_0f(Object *obj) {
    if (game_state.config->field_65 == 0 && ref_other.p->field_6b == 0) {
        func_80131094(obj);
        func_801b6e78_slot04_0f(obj);
        if ((s16)obj->field_3a < 0) {
            obj->field_04 = obj->field_04 + 1;
        }
    }
    func_8011ffdc(obj);
}

void func_801b6e58_slot04_0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801b6e78_slot04_0f(Object *obj) {
    *(s32 *)&obj->field_10 += obj->field_4c;
    *(s32 *)&obj->field_14 += obj->field_50;
}
