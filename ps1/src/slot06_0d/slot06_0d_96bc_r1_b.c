/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea530_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

Object *pop_object_from_second_pool(void);

void call_handler_by_state_0_2_slot06_0d(Object *obj) {
    data_801ea530_slot06_0d[obj->state_0](obj);
}

void spawn_three_child_objects_slot06_0d(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    Object *c;
    int one = 1;
    o->state_0 = o->state_0 + 1;
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->field_02 = 0x19;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->active_count = one;
        c->table_index = 0;
        c->parent = o;
        c->spawn_mode = 4;
        c->image_row = 0;
        *(Object **)&o->linked_object_2 = c;
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->table_index = 1;
        c->active_count = 1;
        c->field_02 = 0x19;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->parent = o;
        c->spawn_mode = 4;
        c->image_row = 0;
        obj->field_30 = (s32)c;
    }
    c = pop_object_from_second_pool();
    if (c != 0) {
        c->table_index = 2;
        c->field_02 = 0x19;
        c->spawn_x = 0x60;
        c->spawn_y = 0x1e0;
        c->active_count = one;
        c->parent = o;
        c->image_row = 0;
        c->spawn_mode = 4;
        o->linked_object_b = (u32)c;
    }
    o->field_0a = 1;
    o->field_0f = 1;
    o->image_row = 0;
    o->field_0c = 0;
    o->spawn_mode = 4;
    set_sequence_entry(o, data_801eb1b0_slot06_0d[4]);
}
