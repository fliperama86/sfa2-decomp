/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7a2c_slot04_11[];

void func_80130dc0(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);
void func_801b6604_slot04_11(Object *obj);
void func_801b6628_slot04_11(Object *obj);
int func_801b6668_slot04_11(Object *obj);
void func_801b66ac_slot04_11(Object *obj);
void func_801b66cc_slot04_11(Object *obj);
void func_801b66ec_slot04_11(Object *obj);
void func_801b6480_slot04_11(Object *obj);

void func_801b5e80_slot04_11(Object *obj) {
    s16 n;
    s16 t;

    obj->state_3 = 3;
    obj->input_dir_0 = 4;
    obj->active_flag_3 = 1;
    func_80130504(obj);
    func_80141f28(obj, *(s16 *)((u8 *)data_801c7a2c_slot04_11 + (obj->direction_index & 0xfe)));
    n = 0xc;
    if (obj->variant_index != 0) {
        n = 0x12;
    }
    if (obj->input_dir_second != 0) {
        n += 3;
    }
    t = obj->direction_index >> 1;
    t += n;
    func_801307e0(obj, t);
    if (obj->input_dir_second != 0 && obj->direction_index == 4) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 5;
        obj->state_3 = 0;
        obj->active_flag = 0;
    }
}

void func_801b5f64_slot04_11(Object *obj) {
    obj->active_flag_3 = 1;
    func_80130dc0(obj);
}

void func_801b5f88_slot04_11(Object *obj) {
    u16 t = obj->step_flags;
    s16 d = t & 0x7f;
    s16 e = d;

    if (d != 0) {
        obj->step_flags = t & 0xff80;
        if (obj->facing_side == 0) {
            e = -d;
        }
        obj->pos_x = e + obj->pos_x;
    }
}
