/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFnInt data_801c61d4_slot04_0e[];
extern ObjectFn data_801c6214_slot04_0e[];
extern u16 data_801c6254_slot04_0e[];
extern u16 data_801c6294_slot04_0e[];
extern Slot04_0eRec62b4 data_801c62b4_slot04_0e[];

void set_record_timer_from_table_plus_random_slot04_0e(Slot04bObj *obj);
void set_countdown_c_random_from_table_slot04_0e(Slot04bObj *obj);

BytePair get_input_byte_pair_or_stored_pair_slot04_0e(Slot04bObj *obj) {
    BytePair d;

    obj->state_flag = 0xff;
    if (obj->active_flag == 0) {
        d = get_direction_pair_from_pressed_bits((Object *)obj);
    } else {
        d.first = obj->countdown_c;
        d.second = obj->field_1dd;
    }
    return d;
}

u8 call_handler_by_limit_hi_returning_slot04_0e(Object *obj) {
    return data_801c61d4_slot04_0e[(obj->limit_hi & 0x1e) >> 1](obj);
}

int tick_record_timer_is_expired_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];

    u16 t = rec->timer_value;

    t = t - 1;
    rec->timer_value = t;
    return (u32)(t << 16) >> 31;
}

int return_zero_3_slot04_0e(Object *obj) {
    return 0;
}

int return_true_always_2_slot04_0e(Object *obj) {
    return 1;
}

int is_vel_y_negative_slot04_0e(Object *obj) {
    return obj->vel_y < 0;
}

int tick_record_timer_set_countdown_by_range_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];
    s16 x;
    u8 k;
    u16 t = rec->timer_value;

    t = t - 1;
    rec->timer_value = t;
    if ((t << 16) >= 0) {
        return 0;
    }
    x = obj->field_21e;
    if (x < 0x58) {
        return 0;
    }
    k = 0;
    if (x >= 0x90) {
        k = 2;
        if (x >= 0xc0) {
            k = 4;
        }
    }
    obj->countdown_c = k;
    return 1;
}

int tick_record_timer_and_check_expired_slot04_0e(Object *o) {
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[o->side];
    u16 t;

    if (((Slot04bObj *)o)->state_flag != 0) {
        return 1;
    }
    t = rec->timer_value;
    t = t - 1;
    rec->timer_value = t;
    if ((t << 16) >= 0) {
        return 0;
    }
    ((Slot04bObj *)o)->countdown_c -= 1;
    if ((((Slot04bObj *)o)->countdown_c & 0x80) != 0) {
        return 0;
    }
    set_record_timer_from_table_plus_random_slot04_0e((Slot04bObj *)o);
    return 1;
}

void call_handler_by_handler_index_if_active_slot04_0e(Slot04bObj *obj) {
    if (obj->active_flag != 0) {
        data_801c6214_slot04_0e[(obj->handler_index & 0x1e) >> 1]((Object *)obj);
    }
}

void reset_record_timer_and_countdown_c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    set_record_timer_from_table_plus_random_slot04_0e(obj);
    set_countdown_c_random_from_table_slot04_0e(obj);
}

void noop_slot00_2_slot04_0e(Object *obj) {
}

void set_countdown_c_from_param_value_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    obj->countdown_c = obj->param_value;
    obj->field_1dd = 0;
}

void call_set_record_timer_from_table_random_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    set_record_timer_from_table_plus_random_slot04_0e(obj);
}

void init_countdown_c_and_scale_record_timer_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Slot04_0eRec62b4 *rec;
    u16 t;

    obj->countdown_c = obj->param_value >> 1;
    obj->field_1dd = 0;
    rec = &data_801c62b4_slot04_0e[obj->field_a6];
    set_record_timer_from_table_plus_random_slot04_0e(obj);
    t = rec->timer_value;
    rec->timer_value = t + (t >> 1);
}

void set_record_timer_from_table_plus_random_slot04_0e(Slot04bObj *obj) {
    Slot04_0eRec62b4 *rec = &data_801c62b4_slot04_0e[obj->field_a6];

    rec->timer_value = data_801c6254_slot04_0e[obj->table_index & 0x1f] + (get_random_byte() & 0x1f);
}

void set_countdown_c_random_from_table_slot04_0e(Slot04bObj *obj) {
    *(u16 *)&obj->countdown_c = data_801c6294_slot04_0e[get_random_byte() & 0xf];
}
