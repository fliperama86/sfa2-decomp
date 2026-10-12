/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea540_slot06_0d[];
extern ObjectFn data_801ea550_slot06_0d[];
extern u8 data_801ea558_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

void set_sequence_from_table_index_or_random_slot06_0d(Object *obj);
void func_8011f38c(Object *o);

void release_object_record_2_slot06_0d(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_0d(Object *obj) {
    data_801ea540_slot06_0d[obj->state_0](obj);
}

void init_child_from_parent_position_slot06_0d(Object *obj) {
    Object *p = obj->parent;
    obj->state_0 = 1;
    obj->field_01 = 1;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->ot_depth = 0x1a;
    obj->field_0e = p->field_0e;
    obj->field_0f = p->field_0f;
    obj->field_1c = p->field_1c;
    obj->scroll_factor = p->scroll_factor;
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
    set_sequence_from_table_index_or_random_slot06_0d(obj);
}

void func_801e9cf0_slot06_0d(Object *obj) {
    Object *p = obj->parent;
    if (game_state.field_65 == 0) {
        data_801ea550_slot06_0d[obj->state_1](obj);
    }
    *(u16 *)&obj->pos_x = *(u16 *)&p->pos_x;
    *(u16 *)&obj->pos_y = *(u16 *)&p->pos_y;
}

void set_random_countdown_on_flag_and_advance_slot06_0d(Object *obj) {
    int v;
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = obj->state_1 + 1;
        v = get_random_byte() & 0x30;
        v |= 0x40;
        obj->countdown = v << (get_random_byte() & 3);
    }
    advance_sequence_checked(obj);
}

void count_down_then_restart_sequence_slot06_0d(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown == 0) {
        obj->state_1 = 0;
        set_sequence_from_table_index_or_random_slot06_0d(obj);
    } else {
        advance_sequence_checked(obj);
    }
}

void set_sequence_from_table_index_or_random_slot06_0d(Object *obj) {
    u8 i = obj->table_index + 5;
    if (obj->table_index == 2) {
        i = data_801ea558_slot06_0d[get_random_byte() & 0xf];
    }
    set_sequence_from_table_5(obj, i, data_801eb1b0_slot06_0d);
}

void unlink_from_parent_and_release_object_slot06_0d(Object *obj) {
    Object *p = obj->parent;
    if (p->linked_object == (u32)obj) {
        p->linked_object = 0;
    } else if (*(Object **)&p->linked_object_2 == obj) {
        *(u32 *)&p->linked_object_2 = 0;
    } else if (((Slot06Obj *)p)->field_30 == (s32)obj) {
        ((Slot06Obj *)p)->field_30 = 0;
    } else if (p->linked_object_b == (u32)obj) {
        p->linked_object_b = 0;
    }
    func_8011f38c(obj);
}
