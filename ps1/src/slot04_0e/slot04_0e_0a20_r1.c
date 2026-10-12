/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_by_side(Object *object, int arg);
void func_80152df4(Object *object);
void func_80130dc0(Object *object);
Block172 *func_8011f1e0(void);
Pooled *func_8011f4a4(void);
int func_80141618(Object *object);
int func_801412a4(Object *object);

void func_801b0bd8_slot04_0e(Object *obj);
void func_801b1270_slot04_0e(Object *obj);
void func_801b12b0_slot04_0e(Object *obj);
void func_801b1488_slot04_0e(Object *obj);
void func_801b16d0_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);
void func_801b6808_slot04_0e(Object *obj);
void func_801b687c_slot04_0e(Object *obj);

extern u8 data_801a27e4[];
extern u16 data_801a2824[];
extern u16 data_801c5da4_slot04_0e[];
extern ObjectFn data_801c5efc_slot04_0e[];
extern ObjectFn data_801c5f0c_slot04_0e[];
extern ObjectFn data_801c5f28_slot04_0e[];
extern ObjectFn data_801c5f34_slot04_0e[];

void func_801b0a20_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 3) {
        obj->mode = 0;
        play_side_sound_at(o, o->field_02, 0x15);
    }
    update_sequence(o);
}


int func_801b0a68_slot04_0e(Object *a, Object *b) {
    *(s32 *)&b->pos_y_frac = *(s32 *)&b->pos_y_frac - a->vel_y;
    return a->vel_y += a->accel_y;
}

void func_801b0a94_slot04_0e(Object *object, int arg) {
    u16 index = arg;
    u16 fi;

    if (object->side == 0) {
        object->sequence = seqs_68_left[index];
    } else {
        object->sequence = seqs_118_right[index];
    }
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    fi = object->sequence->frame_index;
    object->sequence_changed_flag = 1;
    object->frame = object->frames + fi;
}

void func_801b0b10_slot04_0e(Object *object) {
    Object *o = object;
    u16 fi;

    *(s16 *)&o->step_timer = (s16)o->step_timer - 1;
    if ((s16)o->step_timer == 0) {
        if ((s16)o->step_flags < 0) {
            o->sequence = o->sequence + o->sequence->loop_offset;
        } else {
            o->sequence = o->sequence + 1;
        }
        o->step_timer = o->sequence->duration;
        o->step_flags = o->sequence->flags;
        fi = o->sequence->frame_index;
        o->sequence_changed_flag = 1;
        o->frame = o->frames + fi;
    }
}

void func_801b0bb8_slot04_0e(Object *object) {
    func_801378d8(object);
}

void func_801b0bd8_slot04_0e(Object *obj) {
    Rect r;
    Job *job;
    u8 *src;

    src = data_801a27e4 + (obj->image_row << 5);
    r.x = (obj->field_02 << 5) + 0x110;
    r.y = (game_state.frame_counter & 1) + 0x1e0;
    r.w = 0x10;
    r.h = 1;
    func_80158028(&r, src);
    job = (Job *)func_8011f4a4();
    if (job) {
        r.x = 0x60;
        r.y = obj->image_row + 0x1e0;
        r.w = 0x10;
        r.h = 1;
        job->kind = 1;
        job->mode = 2;
        job->rect = r;
        job->src = src;
    }
}

void func_801b0cc8_slot04_0e(void) {
}

void func_801b0cd0_slot04_0e(Object *obj) {
    u16 i;

    for (i = 0; i < 0x10; i++) {
        data_801a2824[(obj->image_row << 4) + i] = data_801c5da4_slot04_0e[i];
    }
    func_80137220(0, 6);
}

void func_801b0d44_slot04_0e(Object *object) {
    data_801c5efc_slot04_0e[object->state_2](object);
}

void func_801b0d84_slot04_0e(Object *object) {
    object->state_2 = object->state_2 + 1;
    object->facing_side = object->saved_facing;
    set_sequence_by_side(object, 0);
}

void func_801b0db8_slot04_0e(Object *object) {
    if (game_state.countdown_timer == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801b0df4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->state_2 = o->state_2 + 1;
    if (game_state_second.table_index == 0) {
        arg = 0x28;
    }
    set_sequence_by_side(o, arg);
}

void func_801b0e40_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    u8 t;

    if (obj->field_47 != 0) {
        t = obj->field_47 - 1;
        obj->field_47 = t;
        if (t == 0) {
            game_state_second.side_bits = game_state.side_bits_2 | (1 << o->side);
        }
    }
    data_801c5f0c_slot04_0e[o->state_3](o);
    update_sequence(o);
}

void func_801b0edc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    Object *p;
    int x;

    if (obj->mode == 1) {
        p = (Object *)func_8011f1e0();
        if (p != 0) {
            p->active_count = 1;
            p->field_02 = 0x3b;
            p->table_index = 7;
            p->ot_depth = 0;
            p->field_02 = 0x14;
            p->team_id = o->team_id;
            p->parent = o;
            p->facing_side = o->facing_side;
            x = 0x33;
            if (o->facing_side == 0) {
                x = -0x33;
            }
            p->pos_x = x + o->pos_x;
            p->pos_y = obj->ground_y - 0x3b;
            obj->mode = 0;
            o->state_3 = o->state_3 + 1;
            p->spawn_x = 0x60;
            p->spawn_y = 0x1e0;
            p->image_row = o->image_row;
            p->mode_id = 0x20;
            p->field_90 = o->field_90;
            p->sound_side = o->sound_side;
            p->data_ptr_a = o->data_ptr_a;
            p->data_ptr_b = o->data_ptr_b;
        }
    }
}

void func_801b0ff0_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 2) {
        obj->mode = 0;
        o->state_3 = o->state_3 + 1;
        play_side_sound_at(o, o->side, 0xc);
        func_801b6808_slot04_0e(o);
    }
}

void func_801b1044_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->mode == 3) {
        obj->mode = 0;
        o->vel_x = 0;
        o->accel_x = 0;
        o->state_3 = o->state_3 + 1;
        func_80152df4(o);
        play_sound_at(o, o->field_02, 0x321);
        o->vel_y = 0x5c000;
        o->accel_y = -0x5400;
        o->active_state = 1;
        func_801308c4(o, 0x13);
        func_801b0bd8_slot04_0e(o);
    }
}

void func_801b10d4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_801b4914_slot04_0e(o);
    if (o->ground_y >= o->pos_y) {
        func_801b0bd8_slot04_0e(o);
    } else {
        obj->countdown = 4;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->state_3 = o->state_3 + 1;
        o->pos_y = obj->ground_y;
        func_801308c4(o, 0x14);
        set_side_flag(o);
        func_801b687c_slot04_0e(o);
    }
}

void func_801b1160_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->countdown--;
    if (obj->countdown == 0) {
        o->active_state = 1;
        o->vel_y = 0x28000;
        o->accel_y = -0x4800;
        o->state_3 = o->state_3 + 1;
        func_801308c4(o, 0x15);
    }
}

void func_801b11c4_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    func_801b4914_slot04_0e(o);
    if (o->ground_y < o->pos_y) {
        o->state_3 = o->state_3 + 1;
        o->active_state = 0;
        o->pos_y_frac = 0;
        o->pos_y = obj->ground_y;
        func_801308c4(o, 0x16);
    }
}

void func_801b1228_slot04_0e(Object *obj) {
}

void func_801b1230_slot04_0e(Object *obj) {
    if (obj->input_dir_0 != 0) {
        func_801b16d0_slot04_0e(obj);
    } else {
        func_801b1270_slot04_0e(obj);
    }
}

void func_801b1270_slot04_0e(Object *obj) {
    obj->field_157 = 0;
    if (obj->input_dir_second != 0) {
        func_801b1488_slot04_0e(obj);
    } else {
        func_801b12b0_slot04_0e(obj);
    }
}

void func_801b12b0_slot04_0e(Object *obj) {
    data_801c5f28_slot04_0e[obj->direction_index >> 1](obj);
}

void func_801b12f4_slot04_0e(Object *obj) {
    data_801c5f34_slot04_0e[obj->state_3](obj);
}

void func_801b1334_slot04_0e(Object *obj) {
    obj->state_3++;
    if (obj->direction_index != 0 && (obj->input_bits_2 & 0xa000) != 0 && (u8)func_8013f8c4(obj, -0x21, 7) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 2;
        obj->state_2 = 0;
        obj->state_3 = 0;
    } else {
        obj->active_flag_3 = 1;
        func_80130dc0(obj);
    }
}

void func_801b13bc_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if ((s16)o->step_flags >= 0) {
        if ((obj->mode & 0x80) != 0 && (u8)func_80141618(o) != 0 || (u8)func_801412a4(o) != 0) {
            o->state_3 = 0;
        }
        update_sequence(o);
    } else {
        func_801312b8(o);
    }
}
