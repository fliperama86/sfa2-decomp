/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80146998(Object *object);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void func_80138b38(GameState *state, Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80130678(Object *object, int arg);
void func_801b1078_slot04_10(Object *obj);
void func_801b63fc_slot04_10(Object *obj);

extern ObjectFn data_801c69d4_slot04_10[];
extern ObjectFn data_801c6a04_slot04_10[];
extern ObjectFn data_801c6a14_slot04_10[];
extern ObjectFn data_801c6a1c_slot04_10[];

int func_801b2324_slot04_10(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->field_177 == 0) {
            return 0;
        }
    }
    if (func_801417cc(obj) != 0) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 1;
        obj->field_159 = 1;
        obj->field_0b = obj->field_158;
    }
    return r;
}

void func_801b23b0_slot04_10(Object *obj) {
    data_801c69d4_slot04_10[obj->field_15a](obj);
}

void func_801b23f0_slot04_10(Object *obj) {
    data_801c6a04_slot04_10[obj->state_3](obj);
}

void func_801b2430_slot04_10(Object *obj) {
    int a;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38((GameState *)game_state.config, obj);
    if (obj->field_45 != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    func_80130678(obj, a);
}

void func_801b249c_slot04_10(Object *obj) {
    s16 a;
    int b;
    if ((s16)obj->step_flags & 0xff00) {
        a = 0;
        b = 0x55;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_45 != 0) {
            a = -0x24;
            b = 0x53;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b63fc_slot04_10(obj);
    }
    update_sequence(obj);
}

void func_801b253c_slot04_10(Object *obj) {
    u8 a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 | 0x1400;
        if (obj->field_4b != 0) {
            obj->field_46 = (obj->field_46 >> 8 << 8) + 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b25d8_slot04_10(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;
    int t;

    obj->field_27c++;
    update_sequence(o);
    a = o->field_46;
    t = a - 1;
    o->field_46 = t;
    if ((t & 0xff) == 0) {
        if (o->field_45 != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    } else {
        if (o->field_134 != 0) {
            if (obj->field_27d == 0) {
                obj->field_27d = obj->field_27c;
            }
        } else if ((s16)(t & -0x100) != 0) {
            o->field_46 = a - 0x101;
            if ((o->field_46 & 0x100) == 0) {
                return;
            }
        } 
        func_80142fe8(o);
    }
}

void func_801b26b0_slot04_10(Object *obj) {
    data_801c6a14_slot04_10[obj->state_3](obj);
}

void func_801b26f0_slot04_10(Object *obj) {
    obj->field_17b = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    func_801307e0(obj, 0x45);
}

void func_801b2744_slot04_10(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b1078_slot04_10(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b2784_slot04_10(Object *obj) {
    data_801c6a1c_slot04_10[obj->state_3](obj);
}

void func_801b27c4_slot04_10(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        func_80146998(obj);
    }
    update_sequence(obj);
}

void func_801b281c_slot04_10(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        if ((u8)func_8013f8c4(obj, -0x26, 0x3a) != 0) {
            obj->state_3++;
            func_80120554(obj, obj->side ^ 1, 0x31a);
        } else {
            obj->state_3 = 3;
            func_801307e0(obj, 0x21);
            return;
        }
    }
    update_sequence(obj);
}

void func_801b28a8_slot04_10(Object *obj) {
    u16 t = obj->step_flags;
    u8 u;
    if ((t & 0xff) == 2) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 1, -0x46, 0x45);
        func_80120554(obj, obj->side ^ 1, 0x304);
    }
    u = obj->step_flags;
    if (u == 1) {
        obj->state_3 = obj->state_3 + 1;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->field_260 = 1;
        func_80140770(obj, 0, 0x11, 0x11, 4, 0, u);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->field_5c < 0) {
            obj->field_167 = 5;
        }
    }
    update_sequence(obj);
}
