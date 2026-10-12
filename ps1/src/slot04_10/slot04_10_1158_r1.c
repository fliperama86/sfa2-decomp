/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 box_margin[];
extern ObjectRef data_80190468;
extern ObjectFn data_801c69ac_slot04_10[];
extern ObjectFn data_801c69c0_slot04_10[];

void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_80140fe0(Object *object);
int func_801410c8(Object *object);
void func_80146478(Object *object, u8 a, int dx, int dy);
void func_80146960(Object *object);
void func_801b1078_slot04_10(Object *obj);

void func_801b1158_slot04_10(Object *obj) {
    int t;

    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    obj->field_0b = 0;
    if (obj->field_cd != 0) {
        t = (s16)(box_margin[0] + 0xc0) < obj->pos_x;
    } else {
        t = obj->field_c2 & 0x2000;
    }
    if (t) {
        obj->field_0b = 1;
    }
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x1c);
}

void func_801b11f8_slot04_10(Object *obj) {
    u16 a = obj->step_flags;

    if ((a & 0xff) != 2) {
        if ((a & 0xff) != 1) {
            update_sequence(obj);
        } else {
            obj->state_3 = obj->state_3 + 1;
            func_80140770(obj, 0, 0xf, -0x200, 0, 0, 0);
            update_sequence(obj);
        }
    } else {
        obj->step_flags = a & 0xff00;
        data_80190468.p->field_63 = 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        func_80140cd8(obj, 0x12, 0);
        play_sound_at(obj, obj->side, 0x319);
    }
}

void func_801b12c8_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801b1308_slot04_10(Object *obj) {
    data_801c69ac_slot04_10[obj->state_3](obj);
}

void func_801b1348_slot04_10(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    obj->gauge = 0x64;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1c0 = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
}

void func_801b13b0_slot04_10(Object *obj) {
    u16 a;
    s16 i;

    ref_other.p = obj->other;
    a = obj->step_flags;
    if ((a & 0xff) == 1) {
        obj->step_flags = a & 0xff00;
        if (func_80146840(obj) != 0) {
            i = -0x4f;
            if (obj->field_0b != 0) {
                i = 0x4f;
            }
            *(u16 *)&ref_other.p->pos_x += i;
            *(u16 *)&ref_other.p->pos_y -= 0x59;
        }
        i = 0;
        if (((Slot04bObj *)obj)->field_1c0 == 0) {
            i = 0xa;
            ((Slot04bObj *)obj)->field_1c0 = ((Slot04bObj *)obj)->field_1c0 + 1;
        }
        if ((u8)func_80140cd8(obj, (s16)i, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x15);
            obj->state_3 = 3;
            func_801307e0(obj, 0x19);
        } else {
            play_side_sound_at(obj, obj->side, 0x13);
            obj->field_46 = obj->field_46 + 1;
            func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            func_801307e0(obj, 0x19);
        }
        }
    } else {
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            func_801307e0(obj, 0x19);
        }
    }
}

void func_801b1590_slot04_10(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, t);
        } else {
            func_80140770(obj, 0, 5, 0xa, 0, 0, t);
        }
        obj->state_3 = 4;
    }
}

void func_801b160c_slot04_10(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, 0);
        } else {
            func_80140770(obj, 0, 5, 0xa, 0, 0, 0);
        }
        obj->state_3 = 4;
    }
}

void func_801b1688_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        func_801b1078_slot04_10(obj);
    }
}

void func_801b16c8_slot04_10(Object *obj) {
    data_801c69c0_slot04_10[obj->state_3](obj);
}

void func_801b1708_slot04_10(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    obj->gauge = 0x64;
    obj->field_46 = 0;
    ((Slot04bObj *)obj)->field_1c0 = 0;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x1a);
}

void func_801b1770_slot04_10(Object *obj) {
    u16 a;
    int i;

    ref_other.p = obj->other;
    a = obj->step_flags;
    if ((a & 0xff) == 1) {
        obj->step_flags = a & 0xff00;
        func_80146478(obj, 1, -0x69, 0x49);
        i = 0;
        if (((Slot04bObj *)obj)->field_1c0 == 0) {
            i = 0xb;
            ((Slot04bObj *)obj)->field_1c0 = ((Slot04bObj *)obj)->field_1c0 + 1;
        }
        if ((u8)func_80140cd8(obj, (s16)i, 0) != 0) {
            play_side_sound_at(obj, obj->side, 0x16);
            obj->state_3 = 3;
            func_801307e0(obj, 0x1b);
        } else {
            play_side_sound_at(obj, obj->side, 0x14);
            obj->field_46 = obj->field_46 + 1;
            func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            func_801307e0(obj, 0x1b);
        }
        }
    } else {
        func_80140fe0(obj);
        if ((s16)obj->field_46 == 0 || (ref_other.p->field_15b != 0 && (u8)func_801410c8(obj) == 0)) {
            update_sequence(obj);
        } else {
            obj->state_3 = 2;
            func_801307e0(obj, 0x1b);
        }
    }
}

void func_801b18f8_slot04_10(Object *obj) {
    u8 t;

    update_sequence(obj);
    t = obj->step_flags;
    if (t == 1) {
        if (((Slot04bObj *)obj)->field_1c0 != 0) {
            func_80140770(obj, 0, 5, 0, 0, 0, t);
        } else {
            func_80140770(obj, 0, 5, 0xb, 0, 0, t);
        }
        obj->state_3 = 4;
    }
}
