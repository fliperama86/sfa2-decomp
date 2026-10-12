/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7798_slot04_11[];
extern u8 data_801c77a8_slot04_11[];
extern ObjectFn data_801c77b8_slot04_11[];

void func_801b5874_slot04_11(Object *obj);
void func_801b5938_slot04_11(Object *obj);
u8 func_801b49b8_slot04_11(Object *obj, int a);
u8 func_801b49e8_slot04_11(Object *obj);
int func_801b5764_slot04_11(Object *obj);
void func_801b4970_slot04_11(Object *obj);

void func_801b4824_slot04_11(Object *obj) {
    Config *c;

    func_801b4970_slot04_11(obj);
    func_801b5764_slot04_11(obj);
    if (obj->pos_y >= obj->ground_y) {
        obj->pos_y = obj->ground_y;
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        func_801209c4(obj);
        if (obj->step_count_2 < obj->step_limit && ((c = game_state.config)->mode_flag_b | c->mode_flag_c | c->mode_flag_a) == 0 && func_8012f56c(obj) == 0) {
            obj->state_3 = 4;
            func_801307e0(obj, 0x4c);
        } else {
            obj->state_3++;
            func_801307e0(obj, 0x6f);
        }
    } else {
        update_sequence(obj);
    }
}

void func_801b4910_slot04_11(Object *obj) {
    if ((obj->step_flags << 16) < 0) {
        ref_other.p = obj->other;
        ref_other.p->other_state_code = 5;
        func_801b5874_slot04_11(obj);
    } else {
        update_sequence(obj);
    }
}

void func_801b4970_slot04_11(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->mode_flag_2 == 0) {
        if (*(u16 *)&obj->input_bits == 0) {
            if (o->pressed_bits != 0) {
                *(u16 *)&obj->input_bits = o->pressed_bits & 0x68;
            }
        }
    }
}

u8 func_801b49b8_slot04_11(Object *obj, int a) {
    if (obj->facing_side != 0) {
        return data_801c7798_slot04_11[a >> 3];
    }
    return a;
}

u8 func_801b49e8_slot04_11(Object *obj) {
    return data_801c77a8_slot04_11[get_random_byte() & 0xf];
}

void func_801b4a1c_slot04_11(Object *obj) {
    func_801b5938_slot04_11(obj);
}

void func_801b4a3c_slot04_11(Object *obj) {
    data_801c77b8_slot04_11[obj->state_3](obj);
}

void func_801b4a7c_slot04_11(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    func_801307e0(obj, 0x20);
}
