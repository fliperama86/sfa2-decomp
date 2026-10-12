/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectRef data_80190468;

int func_80130184(Object *object);
void func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80145f98(Object *object);
void func_80146960(Object *object);
void func_801b10b0_slot04_16(Object *object);
void func_801b10d0_slot04_16(Object *object);
void func_801b4ef8_slot04_16(Object *object);

void func_801b4dbc_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 0;
        obj->active_state = 1;
        func_801b4ef8_slot04_16(obj);
        if (obj->vel_y < 0) {
            obj->state_3++;
            obj->vel_x = 0;
            obj->accel_x = 0;
            func_801307e0(obj, 0x32);
        } else if (func_8013ffe4(obj, -0x3e, 0x18, 0x40, 0x1a) != 0) {
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            func_80145f98(obj);
            if (obj->direction_index == 4) {
                play_side_sound_at(obj, obj->side, 7);
                obj->state_3 = 0xe;
                func_801307e0(obj, 0x37);
            } else if (obj->direction_index == 2) {
                obj->state_3 = 9;
                func_801307e0(obj, 0x34);
            } else {
                obj->state_3 = 6;
                func_801307e0(obj, 0x31);
            }
        }
    }
}

void func_801b4ef8_slot04_16(Object *obj) {
    if (obj->vel_x >= 0) {
        if (obj->facing_side == 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->facing_side != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
}

void func_801b4f80_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->pos_y = obj->ground_y;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b5000_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b505c_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (obj->vel_y >= 0) {
            func_801b4ef8_slot04_16(obj);
        }
        update_sequence(obj);
    } else {
        obj->state_3++;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        func_80140770(obj, 0x1f, 5, 0x16, 0x1a, 1, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 0;
            func_80147000(obj);
        }
        obj->vel_x = 0x10000;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x10000;
        }
        if (obj->vel_y >= 0) {
            obj->vel_y = 0;
        }
        obj->facing_side ^= 1;
        func_801307e0(obj, 0x32);
    }
}

void func_801b51ac_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->active_flag_3 = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b522c_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b10b0_slot04_16(obj);
    }
}

void func_801b5288_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        if (obj->vel_y >= 0) {
            func_801b4ef8_slot04_16(obj);
        }
        update_sequence(obj);
    } else {
        obj->accel_y = 0xffff6000;
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void func_801b5318_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x35);
    }
}

void func_801b5394_slot04_16(Object *obj) {
    u16 v = obj->step_flags;
    u8 st = v;
    if (st == 0) {
        update_sequence(obj);
    } else if (st == 0xf) {
        obj->step_flags = v & 0xff00;
        data_80190468.p->vec_table_index = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        update_sequence(obj);
        play_side_sound_at(obj, obj->side, 8);
        if ((u8)func_80140cd8(obj, 0xd, 0x1b)) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    } else {
        obj->active_state = 1;
        obj->state_3++;
        obj->step_flags &= 0xff00;
        ref_other.p = obj->other;
        ref_other.p->field_15b = 1;
        ref_other.p->hit_flag = 1;
        func_80140770(obj, 0x23, 0xf, -0x200, 0, 0, 0);
        ref_other.p = obj->other;
        if ((s16)ref_other.p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
        }
        obj->vel_x = 0x48000;
        obj->vel_y = 0x90000;
        obj->accel_x = 0;
        obj->accel_y = 0xffff7800;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
}

void func_801b5560_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x36);
    }
}

void func_801b55dc_slot04_16(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        obj->facing_side ^= 1;
        func_801b10d0_slot04_16(obj);
    }
}

void func_801b5648_slot04_16(Object *obj) {
    func_801b4ef8_slot04_16(obj);
    if (obj->vel_y < 0) {
        obj->accel_y = -0x8000;
        obj->state_3++;
        obj->vel_x = 0;
        obj->accel_x = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
        }
    }
    update_sequence(obj);
}

void func_801b56b8_slot04_16(Object *obj) {
    if ((u8)func_80130184(obj)) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        *(u32 *)&obj->pos_y_frac &= 0xffff0000;
        data_80190468.p->vec_table_index = (obj->direction_index << 2) + 0x3c;
        func_80146960(obj);
        func_801307e0(obj, 0x38);
        if ((u8)func_80140cd8(obj, 0xa, 0x1c)) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            data_80190468.p->field_6b = 4;
            func_80147000(obj);
        }
        play_sound_at(obj, obj->side, 0x319);
    }
}
