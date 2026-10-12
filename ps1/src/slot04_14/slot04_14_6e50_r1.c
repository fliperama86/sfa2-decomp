/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern ObjectFn data_801c6650_slot04_14[];
extern u8 data_801c6658_slot04_14[];
extern ObjectFn data_801c665c_slot04_14[];
extern u8 data_801c6664_slot04_14[];
extern s32 data_801c6668_slot04_14[];
extern ObjectFn data_801c6674_slot04_14[];
extern ObjectFn data_801c667c_slot04_14[];
extern ObjectFn data_801c7868_slot04_14[];

void func_801b72b8_slot04_14(Object *obj, u8 index);
void func_801b7460_slot04_14(Object *obj);
void func_801b74b0_slot04_14(Object *obj);
void func_801b7508_slot04_14(Object *obj);
void func_8011f14c(Slab172 *o);
void func_8011f38c(Object *o);
void func_80137be0(Object *o);
void func_80137cc0(Object *o);

void func_801b6e50_slot04_14(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 t = obj->ground_y;
    t -= 0x10;
    if (o->pos_y >= t) {
        o->state_0 = 2;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        func_801b72b8_slot04_14(o, 9);
    } else {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        *(s32 *)&o->pos_y_frac -= o->vel_y;
    }
}

void func_801b6ecc_slot04_14(Object *o) {
    func_80137be0(o);
}

void func_801b6eec_slot04_14(Object *o) {
    func_80137cc0(o);
}

void func_801b6f0c_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c6650_slot04_14[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_801b6f78_slot04_14(Object *o) {
    u8 t;
    o->countdown = 2;
    o->state_2++;
    o->facing_side ^= 1;
    t = data_801c6658_slot04_14[o->vel_table_index];
    if (t != 0) {
        func_801b72b8_slot04_14(o, t);
    }
    advance_sequence_checked(o);
}

void func_801b6fec_slot04_14(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown == 0) {
        o->state_0 = 1;
        o->active_count = 1;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        o->vel_y = 0;
        o->vel_x = -o->vel_x;
    }
    advance_sequence_checked(o);
}

void func_801b704c_slot04_14(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c665c_slot04_14[o->state_2](o);
    }
    func_8011ffdc(o);
}

void func_801b70b8_slot04_14(Object *o) {
    u8 t;
    o->countdown = 2;
    o->state_2++;
    o->facing_side ^= 1;
    t = data_801c6664_slot04_14[o->vel_table_index];
    if (t != 0) {
        func_801b72b8_slot04_14(o, t);
    }
    advance_sequence_checked(o);
}

void func_801b712c_slot04_14(Object *o) {
    o->countdown = (s16)o->countdown - 1;
    if ((s16)o->countdown < 0) {
        o->state_0 = 1;
        o->active_count = 1;
        o->vel_x = -o->vel_x;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
        o->vel_y = data_801c6668_slot04_14[o->vel_table_index];
    }
    advance_sequence_checked(o);
}

void func_801b71a8_slot04_14(Object *o) {
    data_801c6674_slot04_14[o->state_1](o);
    func_8011ffdc(o);
}

void func_801b71fc_slot04_14(Object *o) {
    o->state_1 = o->state_1 + 1;
    func_801b72b8_slot04_14(o, 9);
}

void func_801b7228_slot04_14(Object *o) {
    if ((s16)o->step_flags & 0x8000) {
        o->state_0 = 3;
        o->state_3 = 0;
        o->state_2 = 0;
        o->state_1 = 0;
    }
    advance_sequence_checked(o);
}

void func_801b726c_slot04_14(Object *o) {
    Object *p = o->parent;
    p->link_flags--;
    if (p->link_flags == 0) {
        p->linked_object_3 = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b72b8_slot04_14(Object *obj, u8 index) {
    SequenceStep *e = seqs_a4_left[index + 0x20];
    if (obj->sound_side != 0) {
        e = seqs_154_right[index + 0x20];
    }
    func_80130700(obj, e);
}

void func_801b7310_slot04_14(Object *o) {
    data_801c667c_slot04_14[o->state_0](o);
}

void func_801b7350_slot04_14(Object *o) {
    Object *p = o->parent;
    o->state_0++;
    o->field_1c = p->field_1c;
    o->table_index = p->kind;
    o->field_0c = p->field_0c;
    o->image_row = p->image_row;
    o->field_0e = p->field_0e;
    o->spawn_x = 0x60;
    o->variant_index = 0;
    o->spawn_y = 0x1e0;
    o->ot_depth = 0;
}

void func_801b73b4_slot04_14(Object *o) {
    Object *p = o->parent;
    int s;
    o->field_01 = 0;
    if (o->table_index != p->kind) {
        func_801b7460_slot04_14(o);
    } else {
        s = p->frame->frame_param;
        if (s == 0) {
            o->variant_index = 0;
        } else {
            func_801b7508_slot04_14(o);
            o->field_01 = 1;
            if (o->variant_index != s) {
                o->variant_index = s;
                func_801b74b0_slot04_14(o);
            } else {
                advance_sequence_checked(o);
            }
        }
    }
}

void func_801b7460_slot04_14(Object *o) {
    o->state_0++;
}

void func_801b7474_slot04_14(Object *o) {
    Object *p = o->parent;
    if (p->linked_object == (u32)o) {
        p->linked_object = 0;
    }
    func_8011f38c(o);
}

void func_801b74b0_slot04_14(Object *o) {
    if (o->parent->side == 0) {
        func_80130768(o, o->variant_index, data_1f8000b4);
    } else {
        func_80130768(o, o->variant_index, data_1f800164);
    }
}

void func_801b7508_slot04_14(Object *o) {
    Object *p = o->parent;
    o->pos_x = p->pos_x;
    o->pos_y = p->pos_y;
    o->facing_side = 0;
    if (p->facing_side != 0) {
        o->pos_x = o->pos_x - 5;
    }
}

void func_801b7550_slot04_14(Object *o) {
    data_801c7868_slot04_14[o->state_0](o);
}
