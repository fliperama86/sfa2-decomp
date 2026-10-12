/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801417cc(Object *object);
void func_80138ae8(GameState *state, Object *object);
void func_801b1360_slot04_0d(Object *obj);

extern ObjectFnInt data_801c2980_slot04_0d[];
extern ObjectFn data_801c29a8_slot04_0d[];
extern ObjectFn data_801c29d0_slot04_0d[];
extern ObjectFn data_801c29f8_slot04_0d[];
extern ObjectFn data_801c2a08_slot04_0d[];
extern u8 data_801c2764_slot04_0d[];

int func_801b10cc_slot04_0d(Object *obj) {
    if (obj->field_7e != 0 || obj->remaining_count != 0) {
        if (func_801417cc(obj) != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 7;
            obj->state_3 = 0;
            obj->field_15a = 9;
            obj->active_flag_3 = 1;
            obj->facing_side = obj->saved_facing;
            return 1;
        }
    }
    return 0;
}

void func_801b1158_slot04_0d(Object *obj) {
    data_801ad398 = data_801c2980_slot04_0d[obj->field_15a](obj);
}

int func_801b11a0_slot04_0d(Object *obj) {
    return obj->linked_object_3 == 0;
}

int func_801b11ac_slot04_0d(Object *obj) {
    return (s16)obj->meter >= 0x30;
}

int func_801b11c0_slot04_0d(Object *obj) {
    return 1;
}

int func_801b11c8_slot04_0d(Object *obj) {
    return obj->remaining_count != 0;
}

void func_801b11d4_slot04_0d(Object *obj) {
    data_801c29a8_slot04_0d[obj->field_15a](obj);
}

void func_801b1214_slot04_0d(Object *obj) {
    data_801c29d0_slot04_0d[obj->field_15a](obj);
}

void func_801b1254_slot04_0d(Object *obj) {
    data_801c29f8_slot04_0d[obj->state_3](obj);
}

void func_801b1294_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    update_sequence(o);
    if ((s16)o->step_flags >= 0) {
        if (o->direction_index != 0) {
            obj->active_flag_2 = 1;
        }
    } else {
        if (obj->active_flag_2 == 0 || (obj->active_flag_2 = 0, obj->table_index_3 == 2)) {
            func_801b1360_slot04_0d(o);
        } else {
            obj->table_index_3 = obj->table_index_3 + 1;
            o->direction_index = o->direction_index - 1;
            func_80141f28(o, 1);
            func_801307e0(o, (u8)(data_801c2764_slot04_0d[obj->table_index_3] + 0x28));
        }
    }
}

void func_801b1360_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    o->state_3++;
    t = data_801c2764_slot04_0d[obj->table_index_3];
    o->field_225 = 1;
    o->direction_index = t * 2;
    func_801307e0(o, (u8)(t + 0x2b));
}

void func_801b13b4_slot04_0d(Object *obj) {
    data_801c2a08_slot04_0d[obj->state_3](obj);
}

void func_801b13f4_slot04_0d(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    o->active_flag_4 = 1;
    o->state_3++;
    func_80141f28(o, 4);
    func_80138ae8(&game_state, o);
    obj->active_flag_2 = 0;
    obj->table_index_3 = 0;
    func_801307e0(o, 0x27);
}
