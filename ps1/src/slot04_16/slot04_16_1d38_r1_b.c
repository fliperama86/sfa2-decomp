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
void set_sequence_by_side(Object *object, int index);
void func_801b10b0_slot04_16(Object *obj);
void func_801b6434_slot04_16(Object *obj);

extern ObjectFn data_801ca168_slot04_16[];
extern ObjectFn data_801ca198_slot04_16[];
extern ObjectFn data_801ca1a8_slot04_16[];
extern ObjectFn data_801ca1b0_slot04_16[];
extern ObjectFn data_801ca1c0_slot04_16[];
extern ObjectFn data_801ca1c8_slot04_16[];

int func_801b235c_slot04_16(Object *obj) {
    int r = 0;

    if (obj->field_7e == 0) {
        if (obj->remaining_count == 0) {
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
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

void func_801b23e8_slot04_16(Object *obj) {
    data_801ca168_slot04_16[obj->field_15a](obj);
}

void func_801b2428_slot04_16(Object *obj) {
    data_801ca198_slot04_16[obj->state_3](obj);
}

void func_801b2468_slot04_16(Object *obj) {
    int a;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38((GameState *)game_state.config, obj);
    if (obj->active_state != 0) {
        a = 0x2f;
    } else {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void func_801b24d4_slot04_16(Object *obj) {
    s16 a;
    int b;
    if ((s16)obj->step_flags & 0xff00) {
        a = 0;
        b = 0x55;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -0x24;
            b = 0x53;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b6434_slot04_16(obj);
    }
    update_sequence(obj);
}

void func_801b2574_slot04_16(Object *obj) {
    u8 a;
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->state_3++;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        obj->countdown = (u8)obj->countdown | 0x1400;
        if (obj->field_4b != 0) {
            obj->countdown = (obj->countdown >> 8 << 8) + 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b2610_slot04_16(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int a;
    int t;

    obj->counter++;
    update_sequence(o);
    a = o->countdown;
    t = a - 1;
    o->countdown = t;
    if ((t & 0xff) == 0) {
        if (o->active_state != 0) {
            o->state_0 = 1;
            o->state_1 = 0;
            o->state_2 = 3;
            o->state_3 = 1;
        }
        func_80142c70(o);
    } else {
        if (o->pressed_bits != 0) {
            if (obj->saved_count == 0) {
                obj->saved_count = obj->counter;
            }
        } else if ((s16)(t & -0x100) != 0) {
            o->countdown = a - 0x101;
            if ((o->countdown & 0x100) == 0) {
                return;
            }
        } 
        func_80142fe8(o);
    }
}

void func_801b26e8_slot04_16(Object *obj) {
    data_801ca1a8_slot04_16[obj->state_3](obj);
}

void func_801b2728_slot04_16(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    func_801307e0(obj, 0x45);
}

void func_801b277c_slot04_16(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b10b0_slot04_16(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b27bc_slot04_16(Object *obj) {
    data_801ca1b0_slot04_16[obj->state_3](obj);
}

void func_801b27fc_slot04_16(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        func_80146998(obj);
    }
    update_sequence(obj);
}

void func_801b2854_slot04_16(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        if ((u8)func_8013f8c4(obj, -0x26, 0x3a) != 0) {
            obj->state_3++;
            play_sound_at(obj, obj->side ^ 1, 0x31a);
        } else {
            obj->state_3 = 3;
            func_801307e0(obj, 0x21);
            return;
        }
    }
    update_sequence(obj);
}

void func_801b28e0_slot04_16(Object *obj) {
    u16 t = obj->step_flags;
    u8 u;
    if ((t & 0xff) == 2) {
        obj->step_flags = t & 0xff00;
        func_80146478(obj, 1, -0x46, 0x45);
        play_sound_at(obj, obj->side ^ 1, 0x304);
    }
    u = obj->step_flags;
    if (u == 1) {
        obj->state_3 = obj->state_3 + 1;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        func_80140770(obj, 0, 0x11, 0x11, 4, 0, u);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = 5;
        }
    }
    update_sequence(obj);
}

void func_801b29e4_slot04_16(Object *obj) {
    s16 t;
    int a;

    update_sequence(obj);
    t = obj->step_flags;
    if (t >= 0) {
        if ((u8)t == 3) {
            a = 0x30;
            obj->step_flags = t & 0xff00;
            if (obj->facing_side == 0) {
                a = -0x30;
            }
            obj->pos_x = a + obj->pos_x;
        }
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b2a78_slot04_16(Object *obj) {
    data_801ca1c0_slot04_16[obj->state_3](obj);
}

void func_801b2ab8_slot04_16(Object *obj) {
    if ((u8)obj->step_flags) {
        obj->state_3 = obj->state_3 + 1;
        obj->step_flags &= 0xff00;
        func_80146998(obj);
    }
    update_sequence(obj);
}

void func_801b2b10_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b2b6c_slot04_16(Object *obj) {
    data_801ca1c8_slot04_16[obj->state_3](obj);
}
