/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60fc_slot04_0e[];
extern ObjectFn data_801c6108_slot04_0e[];
extern ObjectFn data_801c6110_slot04_0e[];
extern ObjectFn data_801c611c_slot04_0e[];

void func_80131468(Object *object);
void func_80146998(Object *object);
Block172 *func_8011f1e0(void);
void func_80142fbc(Object *object);
void func_801428a8(Object *object);
void set_sequence_by_side(Object *object, int arg);
void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_80142c70(Object *object);
void func_80138b38(GameState *state, Object *object);

void func_801b4914_slot04_0e(Object *obj);
void func_801b504c_slot04_0e(Object *obj);
void func_801b5088_slot04_0e(Object *obj);
void func_801b577c_slot04_0e(Object *obj);
void func_801b6b40_slot04_0e(Object *obj);

void func_801b5088_slot04_0e(Object *obj) {
    int z = 0;
    u32 *p = (u32 *)((u8 *)obj + 0x2b0);
    u8 i = 0;

    do {
        *p++ = z;
        i++;
    } while (i < 2);
}

void func_801b50b4_slot04_0e(Object *obj) {
    data_801c60fc_slot04_0e[obj->state_3](obj);
}

void func_801b50f4_slot04_0e(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    func_801307e0(obj, 0x42);
}

void func_801b5144_slot04_0e(Object *obj) {
    Object *p;
    int t;
    u8 d;
    if (*(u8 *)&obj->step_flags == 1) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x3b;
            p->table_index = 6;
            p->ot_depth = 0;
            p->field_02 = 0x14;
            p->team_id = obj->team_id;
            p->parent = obj;
            p->facing_side = obj->facing_side;
            t = 0x33;
            if (obj->facing_side == 0) {
                t = -0x33;
            }
            p->pos_x = t + obj->pos_x;
            p->pos_y = ((Slot04bObj *)obj)->ground_y - 0x3b;
            *(u8 *)&obj->step_flags = 0;
            obj->state_3++;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            d = obj->image_row;
            p->mode_id = 0x20;
            p->image_row = d;
            p->field_90 = obj->field_90;
            p->sound_side = obj->sound_side;
            p->data_ptr_a = obj->data_ptr_a;
            p->data_ptr_b = obj->data_ptr_b;
        }
    }
    update_sequence(obj);
}

void func_801b5260_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags > 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        func_801312b8(obj);
    }
}

void func_801b52a0_slot04_0e(Object *obj) {
    data_801c6108_slot04_0e[obj->state_3](obj);
}

void func_801b52e0_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->table_value = 0x19;
    obj->field_157 = 0;
    obj->state_3++;
    func_80146998(obj);
    func_801307e0(obj, 0x40);
}

void func_801b532c_slot04_0e(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
        func_801b504c_slot04_0e(obj);
    } else {
        func_80131468(obj);
    }
}

void func_801b5380_slot04_0e(Object *obj) {
    data_801c6110_slot04_0e[obj->state_3](obj);
}

void func_801b53c0_slot04_0e(Object *obj) {
    ((Slot04bObj *)obj)->table_value = 0x19;
    obj->field_157 = 0;
    obj->state_3++;
    func_80146998(obj);
    func_801307e0(obj, 0x41);
}

void func_801b540c_slot04_0e(Object *obj) {
    update_sequence(obj);
    if ((s16)obj->step_flags < 0) {
        obj->active_state = 1;
        obj->vel_x = 0xc0000;
        obj->accel_x = -0x2000;
        obj->vel_y = 0x40000;
        obj->accel_y = -0x8000;
        obj->state_3++;
        *(u8 *)&obj->step_flags = 0;
        if (obj->facing_side == 0) {
            obj->vel_x = -obj->vel_x;
            obj->accel_x = -obj->accel_x;
        }
    }
}

void func_801b5498_slot04_0e(Object *obj) {
    func_801b4914_slot04_0e(obj);
    if (obj->ground_y < obj->pos_y) {
        obj->active_state = 0;
        obj->pos_x_frac = 0;
        obj->pos_y_frac = 0;
        obj->state_3++;
        obj->pos_y = ((Slot04bObj *)obj)->ground_y;
        func_801209c4(obj);
        obj->facing_side = obj->saved_facing;
        func_80131468(obj);
    }
}

void func_801b550c_slot04_0e(Object *obj) {
    data_801c611c_slot04_0e[obj->state_3](obj);
}

void func_801b554c_slot04_0e(Object *obj) {
    int a;

    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    a = 0x2f;
    if (obj->active_state == 0) {
        a = 0x2b;
    }
    set_sequence_by_side(obj, a);
}

void func_801b55b8_slot04_0e(Object *obj) {
    s16 a;
    int b;

    if (((Slot04bObj *)obj)->mode_bits != 0) {
        obj->field_165 = 0xff;
        ((Slot04bObj *)obj)->field_47 = 0x1e;
        ((Slot04bObj *)obj)->countdown = 1;
        obj->state_3++;
        if (obj->active_state == 0) {
            a = -0x3b;
            b = 0x51;
        } else {
            a = -0xf;
            b = 0x54;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b6b40_slot04_0e(obj);
    }
    update_sequence(obj);
}

void func_801b5660_slot04_0e(Object *obj) {
    int a;
    ((Slot04bObj *)obj)->field_47--;
    if (((Slot04bObj *)obj)->field_47 == 0) {
        obj->state_3++;
        ((Slot04bObj *)obj)->field_47 = 0x14;
        a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        obj->hit_count_3 = 0;
        ((Slot04bObj *)obj)->saved_count = 0;
        if (obj->field_4b != 0) {
            a = 0x48;
        }
        obj->cursor_timer_a = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b56e8_slot04_0e(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown != 0) {
        func_801b577c_slot04_0e(obj);
    } else {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        func_80142c70(obj);
    }
}
