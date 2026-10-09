/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern ObjectFn data_801c6650_slot04_14[];
extern u8 data_801c6658_slot04_14[];
extern ObjectFn data_801c665c_slot04_14[];
extern u8 data_801c6664_slot04_14[];
extern s32 data_801c6668_slot04_14[];
extern ObjectFn data_801c6674_slot04_14[];
extern ObjectFn data_801c667c_slot04_14[];
extern ObjectFn data_801c7868_slot04_14[];

void func_801b72b8_slot04_14(Object *obj, u8 index);
void func_801b7460_slot04_14(Object *obj);
void func_801b74b0_slot04_14(Object *obj);
void func_801b7508_slot04_14(Object *obj);
void func_8011f14c(Slab172 *o);
void func_8011f38c(Object *o);
void func_80137be0(Object *o);
void func_80137cc0(Object *o);

void func_801b6e50_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 t = obj->field_70;
    t -= 0x10;
    if (o->pos_y >= t) {
        o->field_04 = 2;
        o->field_07 = 0;
        o->field_06 = 0;
        o->field_05 = 0;
        func_801b72b8_slot04_14(o, 9);
    } else {
        *(s32 *)&o->field_10 += o->field_4c;
        *(s32 *)&o->field_14 -= o->field_50;
    }
}

void func_801b6ecc_slot04_14(Object *o) {
    func_80137be0(o);
}

void func_801b6eec_slot04_14(Object *o) {
    func_80137cc0(o);
}

void func_801b6f0c_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c6650_slot04_14[o->field_06](o);
    }
    func_8011ffdc(o);
}

void func_801b6f78_slot04_14(Object *o) {
    u8 t;
    o->field_46 = 2;
    o->field_06++;
    o->field_0b ^= 1;
    t = data_801c6658_slot04_14[o->field_ac];
    if (t != 0) {
        func_801b72b8_slot04_14(o, t);
    }
    func_80131094(o);
}

void func_801b6fec_slot04_14(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 == 0) {
        o->field_04 = 1;
        o->field_00 = 1;
        o->field_07 = 0;
        o->field_06 = 0;
        o->field_05 = 0;
        o->field_50 = 0;
        o->field_4c = -o->field_4c;
    }
    func_80131094(o);
}

void func_801b704c_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c665c_slot04_14[o->field_06](o);
    }
    func_8011ffdc(o);
}

void func_801b70b8_slot04_14(Object *o) {
    u8 t;
    o->field_46 = 2;
    o->field_06++;
    o->field_0b ^= 1;
    t = data_801c6664_slot04_14[o->field_ac];
    if (t != 0) {
        func_801b72b8_slot04_14(o, t);
    }
    func_80131094(o);
}

void func_801b712c_slot04_14(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((s16)o->field_46 < 0) {
        o->field_04 = 1;
        o->field_00 = 1;
        o->field_4c = -o->field_4c;
        o->field_07 = 0;
        o->field_06 = 0;
        o->field_05 = 0;
        o->field_50 = data_801c6668_slot04_14[o->field_ac];
    }
    func_80131094(o);
}

void func_801b71a8_slot04_14(Object *o) {
    data_801c6674_slot04_14[o->field_05](o);
    func_8011ffdc(o);
}

void func_801b71fc_slot04_14(Object *o) {
    o->field_05 = o->field_05 + 1;
    func_801b72b8_slot04_14(o, 9);
}

void func_801b7228_slot04_14(Object *o) {
    if ((s16)o->field_3a & 0x8000) {
        o->field_04 = 3;
        o->field_07 = 0;
        o->field_06 = 0;
        o->field_05 = 0;
    }
    func_80131094(o);
}

void func_801b726c_slot04_14(Object *o) {
    Object *p = o->field_3c;
    p->field_240--;
    if (p->field_240 == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b72b8_slot04_14(Object *obj, u8 index) {
    SequenceStep *e = seqs_a4_left[index + 0x20];
    if (obj->field_66 != 0) {
        e = seqs_154_right[index + 0x20];
    }
    func_80130700(obj, e);
}

void func_801b7310_slot04_14(Object *o) {
    data_801c667c_slot04_14[o->field_04](o);
}

void func_801b7350_slot04_14(Object *o) {
    Object *p = o->field_3c;
    o->field_04++;
    o->field_1c = p->field_1c;
    o->field_03 = p->kind;
    o->field_0c = p->field_0c;
    o->field_0d = p->field_0d;
    o->field_0e = p->field_0e;
    o->field_7a = 0x60;
    o->field_48 = 0;
    o->field_7c = 0x1e0;
    o->field_09 = 0;
}

void func_801b73b4_slot04_14(Object *o) {
    Object *p = o->field_3c;
    int s;
    o->field_01 = 0;
    if (o->field_03 != p->kind) {
        func_801b7460_slot04_14(o);
    } else {
        s = p->frame->field_09;
        if (s == 0) {
            o->field_48 = 0;
        } else {
            func_801b7508_slot04_14(o);
            o->field_01 = 1;
            if (o->field_48 != s) {
                o->field_48 = s;
                func_801b74b0_slot04_14(o);
            } else {
                func_80131094(o);
            }
        }
    }
}

void func_801b7460_slot04_14(Object *o) {
    o->field_04++;
}

void func_801b7474_slot04_14(Object *o) {
    Object *p = o->field_3c;
    if (p->field_28 == (u32)o) {
        p->field_28 = 0;
    }
    func_8011f38c(o);
}

void func_801b74b0_slot04_14(Object *o) {
    if (o->field_3c->side == 0) {
        func_80130768(o, o->field_48, data_1f8000b4);
    } else {
        func_80130768(o, o->field_48, data_1f800164);
    }
}

void func_801b7508_slot04_14(Object *o) {
    Object *p = o->field_3c;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->field_0b = 0;
    if (p->field_0b != 0) {
        o->pos_x = o->pos_x - 5;
    }
}

void func_801b7550_slot04_14(Object *o) {
    data_801c7868_slot04_14[o->field_04](o);
}
