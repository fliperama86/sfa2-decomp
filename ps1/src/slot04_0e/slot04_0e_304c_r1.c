/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c6050_slot04_0e[];

void func_80142adc(Object *object);
void func_80138ae8(GameState *state, Object *object);

void func_801b2f58_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
u8 func_801b6a7c_slot04_0e(Object *obj);
void func_801b6afc_slot04_0e(Object *obj);
BytePair func_801b6b64_slot04_0e(Slot04bObj *obj);
u8 func_801b6bd4_slot04_0e(Object *obj);

void func_801b3284_slot04_0e(Object *obj);
void func_801b31b8_slot04_0e(Object *obj, int arg);
void func_801b3200_slot04_0e(Object *obj);
void func_801b339c_slot04_0e(Object *obj);

void func_801b304c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->state_flag == 0) {
        func_801b3284_slot04_0e(o);
    }
    func_801b4914_slot04_0e(o);
    func_801b31b8_slot04_0e(o, 3);
    if (obj->state_flag != 0) {
        o->state_3 = o->state_3 + 1;
        o->field_28a = 1;
        func_801b3200_slot04_0e(o);
    } else if (o->ground_y < o->pos_y) {
        func_801b2f58_slot04_0e(o);
    } else {
        update_sequence(o);
    }
}

void func_801b30fc_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    func_801b31b8_slot04_0e(obj, 5);
    if (obj->ground_y < obj->pos_y) {
        func_801b2f58_slot04_0e(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b3160_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        func_80142adc(obj);
        update_sequence(obj);
    } else {
        obj->field_28a = 0;
        func_801312b8(obj);
    }
}

void func_801b31b8_slot04_0e(Object *obj, int arg) {
    u16 d = arg;

    if (obj->mode_flag_2 == 0 && (obj->input_bits & 0xa000) != 0) {
        if (obj->input_bits & 0x8000) {
            d = -d;
        }
        *(u16 *)&obj->pos_x = d + *(u16 *)&obj->pos_x;
    }
}

void func_801b3200_slot04_0e(Object *obj) {
    u16 a;
    u16 h;

    if (func_801b6a7c_slot04_0e(obj) != 0) {
        func_801b6afc_slot04_0e(obj);
    } else {
        h = obj->direction_index >> 1;
        func_80141f28(obj, h);
        a = 0x15;
        if (obj->input_dir_second == 0) {
            a = 0x12;
        }
        func_801307e0(obj, a += h);
    }
}

void func_801b3284_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    BytePair buf;
    int k;

    if (o->mode_flag_2 != 0) {
        k = func_801b6bd4_slot04_0e(o);
    } else {
        k = obj->field_134;
    }
    if (k != 0) {
        buf = func_801b6b64_slot04_0e((Slot04bObj *)o);
        o->direction_index = buf.first;
        o->input_dir_second = buf.second;
    }
}

void func_801b32f8_slot04_0e(Object *obj) {
    data_801c6050_slot04_0e[obj->state_3](obj);
}

void func_801b3338_slot04_0e(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    func_80141f28(obj, 2);
    obj->accel_y -= 0x6000;
    func_80138ae8(&game_state, obj);
    func_801b339c_slot04_0e(obj);
}
