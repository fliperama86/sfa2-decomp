/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7388_slot04_11[];
extern u8 data_801c7398_slot04_11[];
extern u16 data_801c739c_slot04_11[];
extern ObjectFn data_801c73a4_slot04_11[];
extern s32 data_801c73c0_slot04_11[];
extern u8 data_801c73f0_slot04_11[];
extern u8 data_801c73f8_slot04_11[];
extern u8 data_801c701c_slot04_11[];
extern u8 data_801c709c_slot04_11[];
void func_801b15c4_slot04_11(Object *obj);
void func_801b180c_slot04_11(Object *obj);
void func_801b1850_slot04_11(Object *obj);
void func_801b17c0_slot04_11(Object *obj);
void func_801b1c58_slot04_11(Object *obj);
void func_801b1f54_slot04_11(Object *obj);
void func_801b5874_slot04_11(Object *obj);
void func_80142adc(Object *object);
int func_801b57a8_slot04_11(Object *obj);

void func_801b1488_slot04_11(Object *obj) {
    data_801c7388_slot04_11[obj->field_07](obj);
}

void func_801b14c8_slot04_11(Object *obj) {
    int a;

    obj->field_17b = 1;
    obj->field_12d = 0xb4;
    obj->field_07++;
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

void func_801b1544_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a != 0) {
        o->field_46 = 0xf;
        o->field_07++;
        if (o->field_cd == 0) {
            func_801b15c4_slot04_11(o);
        } else {
            o->field_46 = data_801c7398_slot04_11[o->field_129];
            func_801b1850_slot04_11(o);
        }
    } else {
        func_80130efc(o);
    }
}

void func_801b15c4_slot04_11(Object *obj) {
    s16 x;
    int h;
    u8 s;

    h = (s16)obj->field_3a >> 8;
    if (obj->field_cd != 0) {
        func_801b1850_slot04_11(obj);
    } else if ((*(u32 *)&game_state.field_4c & 0xffff00) != 0 || game_state.field_04 != 0 ||
               (func_8012f56c(obj) & 0xff) != 0) {
        func_801b180c_slot04_11(obj);
    } else {
        if ((h & 0x7f) != 0) {
            obj->field_3a = obj->field_3a & 0x80ff;
            func_80142c04(obj);
            func_80138ae8(&game_state, obj);
            if (obj->field_254 == 0) {
                obj->field_254 = 0xff;
            }
        }
        if ((obj->field_134 & 0x94) == 0) {
            func_801b17c0_slot04_11(obj);
        } else {
            if ((obj->field_134 & 0x80) != 0) {
                x = 0;
            } else if ((obj->field_134 & 0x10) != 0) {
                x = 2;
            } else {
                x = 4;
            }
            if (obj->field_46 <= data_801c739c_slot04_11[x >> 1]) {
                func_801b17c0_slot04_11(obj);
            } else {
                obj->field_46 = 0xf;
                if (obj->field_12a == x) {
                    func_80130efc(obj);
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
                    obj->field_3a = obj->sequence->flags;
                    obj->field_38 = obj->sequence->duration;
                    obj->frame = obj->frames + obj->sequence->frame_index;
                    obj->field_4a = obj->frame->field_0d;
                    obj->field_80 = 1;
                }
            }
        }
    }
}

void func_801b17c0_slot04_11(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        func_80130efc(obj);
    } else {
        func_801b180c_slot04_11(obj);
    }
}

void func_801b180c_slot04_11(Object *obj) {
    int a;

    obj->field_07++;
    obj->field_17b = 0;
    a = 0x22;
    if (obj->field_49 != 0) {
        a = 0x41;
    }
    a += obj->field_12a;
    func_801307e0(obj, a);
}

void func_801b1850_slot04_11(Object *obj) {
    u16 t;

    if ((game_state.config->field_4d | game_state.config->field_4e | game_state.config->field_04) != 0 ||
        (func_8012f56c(obj) & 0xff) != 0) {
        goto tail;
    }
    t = obj->field_3a;
    if (((t >> 8) & 0x7f) != 0 && (s16)obj->field_38 == 1) {
        obj->field_3a = t & 0x80ff;
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
        } else if ((func_8014a170(obj, (u32 *)data_801c701c_slot04_11) & 0xff) != 0) {
            goto tail;
        }
    }
    func_80130efc(obj);
    return;
tail:
    obj->field_07++;
    obj->field_17b = 0;
    func_801307e0(obj, obj->field_12a + 0x22);
}

void func_801b1984_slot04_11(Object *obj) {
    if ((obj->field_3a << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b5874_slot04_11(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b19f8_slot04_11(Object *obj) {
    data_801c73a4_slot04_11[obj->field_07](obj);
}

void func_801b1a38_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int d;

    o->field_17b = 1;
    o->field_07++;
    func_80141f28(o, 9);
    func_80138ae8(&game_state, o);
    o->field_67 = 0;
    o->field_12f = 0;
    o->field_4c = data_801c73c0_slot04_11[o->field_12a * 2];
    o->field_54 = data_801c73c0_slot04_11[o->field_12a * 2 + 1];
    o->field_50 = data_801c73c0_slot04_11[o->field_12a * 2 + 2];
    o->field_58 = data_801c73c0_slot04_11[o->field_12a * 2 + 3];
    d = 0x10;
    if (o->field_0b == 0) {
        d = -0x10;
    }
    o->pos_x = o->pos_x + d;
    func_801204f4(o, obj->field_a6, 5);
    if (o->field_49 == 0) {
        func_801b1f54_slot04_11(o);
    } else {
        func_801307e0(o, (o->field_12a >> 1) + 0x46);
    }
}

void func_801b1b6c_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
        o->field_12e = data_801c73f0_slot04_11[o->field_12a & 0xfe];
    }
    func_80130efc(o);
}

void func_801b1bcc_slot04_11(Object *obj) {
    if (obj->field_67 != 0) {
        obj->field_4c = 0xc000;
        obj->field_50 = 0x19000;
        obj->field_54 = 0;
        obj->field_58 = -0x1000;
        obj->field_46 = 0x1801;
        obj->field_07++;
    }
    if (func_801b57a8_slot04_11(obj) < 0) {
        func_801b1c58_slot04_11(obj);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1c58_slot04_11(Object *obj) {
    int a = -0x6000;

    obj->field_07 = 5;
    if (obj->field_49 != 0) {
        a = -0x12000;
    }
    obj->field_58 = a;
    func_801307e0(obj, 0x3c);
}

void func_801b1c98_slot04_11(Object *o) {
    u8 t;

    if (func_801b57a8_slot04_11(o) < 0 || ((Slot04bObj *)o)->field_47 == 0) {
        func_801b1c58_slot04_11(o);
    } else {
        ((Slot04bObj *)o)->field_47--;
        ((Slot04bObj *)o)->field_46++;
        if (o->field_67 != 0) {
            if (o->field_cd != 0 ? (((Slot04bObj *)o)->field_47 < 6 &&
                                    (func_8014a170(o, (u32 *)data_801c709c_slot04_11) & 0xff) == 0)
                                 : ((o->field_134 & data_801c73f8_slot04_11[o->field_12a >> 1]) != 0)) {
                o->field_12f++;
                t = ((Slot04bObj *)o)->field_46;
                if (t != 0) {
                    t--;
                }
                ((Slot04bObj *)o)->field_47 = t;
                func_801204f4(o, o->side, 6);
                ((Slot04bObj *)o)->field_46 = 0;
                o->field_67 = 0;
                if (o->field_12f == o->field_12e) {
                    o->field_54 = -0x2000;
                    o->field_58 = -0x5000;
                    o->field_07++;
                }
                o->field_4c = 0xc000;
                o->field_50 = 0x19000;
                func_801b1f54_slot04_11(o);
            }
        }
        func_80130efc(o);
    }
}

void func_801b1dfc_slot04_11(Object *obj) {
    int a;

    if (func_801b57a8_slot04_11(obj) < 0) {
        a = -0x7000;
        obj->field_07++;
        if (obj->field_49 != 0) {
            a = -0x15000;
        }
        obj->field_58 = a;
    }
    func_80130efc(obj);
}
