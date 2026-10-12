/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80142ba0(Object *object);

extern ObjectFnInt data_801c5a30_slot04_0f[];
extern ObjectFn data_801c5a60_slot04_0f[];
extern ObjectFn data_801c5a90_slot04_0f[];

void func_801b187c_slot04_0f(Object *obj);
void func_801b16b4_slot04_0f(Object *obj);
int func_801b1c74_slot04_0f(Object *obj);

void func_801b16b4_slot04_0f(Object *obj) {
    u8 t = 1;

    obj->state_0 = t;
    obj->active_flag_3 = t;
    t = obj->saved_facing;
    obj->state_2 = 7;
    obj->field_15a = 9;
    obj->other_lock = 0x1d;
    obj->facing_side = t;
    obj->state_1 = 0;
    obj->state_3 = 0;
    obj->field_157 = 0;
    obj->field_6b = 0;
    obj->other->field_6b = 0x19;
    func_801307e0(obj, 0x48);
}

int func_801b171c_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;

    if (o->active_state != 0) {
        if (p->timer != 0) return 0;
        ok = (u8)func_801418bc(o);
    } else {
        ok = func_801417cc(o);
    }
    if (ok) {
        o->state_0 = 1;
        o->state_1 = 0;
        o->state_2 = 7;
        o->state_3 = 0;
        o->field_15a = 0;
        o->variant_index = o->facing_side = o->saved_facing;
        p[1].stage = 0;
        func_801b187c_slot04_0f(o);
        return 1;
    }
    return 0;
}

int func_801b17c8_slot04_0f(Object *o) {
    Slot *p = &o->slots[16];
    int ok;
    u8 t;

    if (o->active_state != 0) {
        if (p->timer != 0) return 0;
        ok = (u8)func_801418bc(o);
    } else {
        ok = func_801417cc(o);
    }
    if (ok) {
        o->state_0 = 1;
        o->state_2 = 7;
        o->state_1 = 0;
        o->state_3 = 0;
        o->field_15a = 0;
        t = o->saved_facing;
        o->facing_side = t;
        o->variant_index = t ^ 1;
        p[1].stage = 4;
        func_801b187c_slot04_0f(o);
        return 1;
    }
    return 0;
}

void func_801b187c_slot04_0f(Object *obj) {
    u16 t;

    t = obj->pressed_bits | obj->released_bits;
    obj->direction_index = 0;
    obj->active_flag_3 = 0;
    if ((t & 0x68) == 0x68 || (t & 2) != 0) {
        obj->slots[17].stage = obj->slots[17].stage + 2;
    }
}

int func_801b18c0_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = one;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    func_80142b3c(obj);
    return 1;
}

int func_801b1958_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 2;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    func_80142b3c(obj);
    return 1;
}

int func_801b19f4_slot04_0f(Object *obj) {
    u8 one;
    u8 t;

    if (obj->field_7e == 0 && obj->link_flags != 0) return 0;
    if (!func_801417cc(obj)) return 0;
    one = 1;
    obj->state_0 = one;
    obj->state_1 = 0;
    obj->state_2 = 7;
    obj->state_3 = 0;
    obj->field_15a = 3;
    obj->input_dir_second = 0;
    obj->active_flag_3 = one;
    t = obj->saved_facing;
    obj->facing_side = t;
    obj->variant_index = t;
    func_80142ba0(obj);
    return 1;
}

void func_801b1a90_slot04_0f(Object *obj) {
    data_801ad398 = data_801c5a30_slot04_0f[obj->field_15a](obj);
}

int func_801b1ad8_slot04_0f(Object *obj) {
    u8 t;

    t = obj->saved_facing;
    obj->slots[17].stage = 0;
    obj->facing_side = t;
    obj->variant_index = t;
    if (obj->direction_flag != 0) {
        obj->slots[17].stage = 4;
    }
    if (obj->input_dir_second != 0) {
        obj->slots[17].stage = obj->slots[17].stage + 2;
    }
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1b38_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    if (obj->link_flags != 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1b7c_slot04_0f(Object *obj) {
    if (obj->link_flags != 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1bac_slot04_0f(Object *obj) {
    if ((s16)obj->meter < 0x30) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1be0_slot04_0f(Object *obj) {
    if (obj->remaining_count == 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1c10_slot04_0f(Object *obj) {
    if (*(s32 *)&obj->state_0 != 0x2020202) return 0;
    if (obj->active_state != 0) return 0;
    if (obj->field_163 != 0) return 0;
    if ((s16)obj->health < 0) return 0;
    return func_801b1c74_slot04_0f(obj);
}

int func_801b1c74_slot04_0f(Object *obj) {
    return 1;
}

int func_801b1c7c_slot04_0f(Object *obj) {
    return 0;
}

void func_801b1c84_slot04_0f(Object *obj) {
    data_801c5a60_slot04_0f[obj->field_15a](obj);
}

void func_801b1cc4_slot04_0f(Object *obj) {
    data_801c5a90_slot04_0f[obj->field_15a](obj);
}
