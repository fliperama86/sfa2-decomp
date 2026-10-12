/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80026908_slot27[])(Object *);
extern Slot27Rec9338 data_80029338_slot27[];

void func_800119c8_slot27(Object *obj) {
    ref_other.p = obj->other;
    data_80026908_slot27[obj->state_0](obj);
}

void func_80011a18_slot27(Object *obj) {
    obj->box_tables = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->field_60 = 0;
    obj->hit_type = 0;
    obj->active_state = 0;
    obj->sound_side = 0;
    obj->health = 0;
    obj->y_offset = 0;
    obj->accel_x = 0;
    obj->accel_y = 0;
    obj->field_62 = 0;
    obj->vec_table_index = 0;
    obj->ground_y = 0;
    obj->status_flag = 0;
    obj->facing_flag = 0;
    obj->field_62 = 0;
    obj->field_6b = 0;
}

void func_80011a64_slot27(void) {
    Slot27Rec9338 *r = &data_80029338_slot27[ref_other.p->side];

    r->slot_a = 0;
    r->slot_b = 0;
    r->step_count = 0;
    r->slot_c = 0;
    r->slot_d = 0;
    r->count_a = 0;
    r->count_b = 0;
}
