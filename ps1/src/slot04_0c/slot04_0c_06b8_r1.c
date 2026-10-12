/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_8013d210(Object *object);
int func_8013d1a8(Object *object);
int func_8013cb70(Object *object, u8 index, u8 arg);
int func_8013cbdc(Object *object, u8 index, u8 arg);
int func_8013cc48(Object *object, u8 index, u8 arg);
u8 func_801417cc(Object *object);
int func_80141e34(Object *object);
int func_80141b28(Object *object);
void func_80142718(Object *object);
void func_80142778(Object *object);
/* functions of other units of this module */
int func_801b0f9c_slot04_0c(Object *obj);
int func_801b0f18_slot04_0c(Object *obj);
int func_801b1010_slot04_0c(Object *obj);
void func_801b0eb0_slot04_0c(Object *obj);

int func_801b08d4_slot04_0c(Object *obj);
int func_801b0958_slot04_0c(Object *obj);
int func_801b09bc_slot04_0c(Object *obj);
void func_801b0a24_slot04_0c(Object *obj);
int func_801b0a3c_slot04_0c(Object *obj);
int func_801b0b30_slot04_0c(Object *obj);
int func_801b0bf4_slot04_0c(Object *obj);
int func_801b0c80_slot04_0c(Object *obj);
int func_801b0d1c_slot04_0c(Object *obj);
int func_801b0da8_slot04_0c(Object *obj);
void func_801b0df8_slot04_0c(Object *obj);
int func_801b0e60_slot04_0c(Object *obj);

void func_801b06b8_slot04_0c(Object *obj) {
    if (func_8013d21c(obj, 8, 0x18) && func_801b08d4_slot04_0c(obj)) return;
    if (func_8013d21c(obj, 9, 0x1f) && func_801b0958_slot04_0c(obj)) return;
    if (func_8013d21c(obj, 10, 0x20) && func_801b09bc_slot04_0c(obj)) return;
    if (func_8013d210(obj) && func_801b0b30_slot04_0c(obj)) return;
    if ((u8)func_8013d1a8(obj) && func_801b0a3c_slot04_0c(obj)) return;
    if ((u8)func_8013cb70(obj, 0, 0x14) && func_801b0c80_slot04_0c(obj)) return;
    if ((u8)func_8013cbdc(obj, 1, 0x15) && func_801b0d1c_slot04_0c(obj)) return;
    if ((u8)func_8013cc48(obj, 2, 0x17) && func_801b0bf4_slot04_0c(obj)) return;
    if ((u8)func_8013de2c(obj, 3, 4) && func_801b0f9c_slot04_0c(obj)) return;
    if ((u8)func_8013de2c(obj, 4, 0) && func_801b0f18_slot04_0c(obj)) return;
    if ((u8)func_8013de2c(obj, 5, 3) && func_801b1010_slot04_0c(obj)) return;
    if ((u8)func_8013de2c(obj, 6, 0xe) && func_801b0da8_slot04_0c(obj)) return;
    if ((u8)func_8013de2c(obj, 7, 0xd)) func_801b0e60_slot04_0c(obj);
}

int func_801b08d4_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 0xc;
        obj->active_flag_3 = 0;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int func_801b0958_slot04_0c(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        func_801b0a24_slot04_0c(obj);
    }
    return r;
}

int func_801b09bc_slot04_0c(Object *obj) {
    int r = 0;

    if (func_801417cc(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_2 = 7;
        obj->state_1 = 0;
        obj->state_3 = 3;
        func_801b0a24_slot04_0c(obj);
    }
    return r;
}

void func_801b0a24_slot04_0c(Object *obj) {
    obj->facing_side = obj->saved_facing;
    obj->field_15a = 10;
    obj->active_flag_3 = 0;
}

int func_801b0a3c_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->field_292 == 0) {
        if ((s16)obj->meter >= 0x30) {
            if (obj->active_state == 0) {
                if ((u8)func_80141e34(obj)) {
                    if ((u8)func_80141788(obj)) {
                        r = 1;
                        obj->state_0 = 1;
                        obj->state_1 = 0;
                        obj->state_2 = 8;
                        obj->state_3 = 0;
                        obj->field_15a = 8;
                        obj->facing_side = obj->saved_facing;
                    }
                }
            } else if (obj->field_7e == 0) {
                if ((u8)func_801418bc(obj)) {
                    r = 1;
                    obj->state_0 = 1;
                    obj->state_1 = 0;
                    obj->state_2 = 8;
                    obj->state_3 = 0;
                    obj->field_15a = 8;
                }
            }
        }
    }
    return r;
}

int func_801b0b30_slot04_0c(Object *obj) {
    int r = 0;

    if (obj->active_state != 0) {
        if ((obj->vel_y & 0xffff0000) != 0) {
            if ((u8)func_801418bc(obj)) {
                r = 1;
                obj->state_0 = 1;
                obj->state_2 = 7;
                obj->state_1 = 0;
                obj->state_3 = 0;
                obj->field_15a = 9;
            }
        }
    } else if (func_801417cc(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 7;
        obj->state_3 = 0;
        obj->field_15a = 7;
        obj->facing_side = obj->saved_facing;
    }
    return r;
}

int func_801b0bf4_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 0xb;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
    return r;
}

int func_801b0c80_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30) {
        if (obj->link_flags == 0) {
            if ((u8)func_80141788(obj)) {
                r = 1;
                obj->state_0 = 1;
                obj->state_1 = 0;
                obj->state_2 = 8;
                obj->state_3 = 0;
                obj->field_15a = 4;
                obj->active_flag_3 = 1;
                obj->facing_side = obj->saved_facing;
                func_80142718(obj);
            }
        }
    }
    return r;
}

int func_801b0d1c_slot04_0c(Object *obj) {
    int r = 0;

    if ((s16)obj->meter >= 0x30 && (u8)func_80141788(obj)) {
        r = 1;
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 8;
        obj->state_3 = 0;
        obj->field_15a = 6;
        obj->active_flag_3 = 1;
        obj->facing_side = obj->saved_facing;
        func_80142778(obj);
    }
    return r;
}

int func_801b0da8_slot04_0c(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b0df8_slot04_0c(obj);
    }
    return r;
}

void func_801b0df8_slot04_0c(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->facing_side = t;
    obj->field_15a = 2;
    obj->other_lock = 0x1b;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x17;
    func_801307e0(obj, 0x1a);
}

int func_801b0e60_slot04_0c(Object *obj) {
    int r = 0;

    if ((u8)func_80141b28(obj)) {
        r = 1;
        func_801b0eb0_slot04_0c(obj);
    }
    return r;
}
