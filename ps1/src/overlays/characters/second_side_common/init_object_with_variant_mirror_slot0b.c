/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern HudSlot data_801e7648_slot0b[];
extern int data_801e4cfc_slot0b[];
void init_hud_slot_for_other_side_slot0b(Object *obj, Object *other);
void set_hud_slot_position_from_object_slot0b(Object *obj, Object *other);

void init_object_with_variant_mirror_slot0b(Object *obj) {
    Object *o;
    obj->field_0a = 1;
    obj->countdown = 0x1f;
    obj->pos_x = -0xb8;
    obj->pos_y = 0xc9;
    obj->health = 0x48;
    obj->vel_x = 0x80000;
    obj->field_0e = 0;
    obj->ot_depth = 0;
    obj->table_index_2 = 0;
    obj->facing_side = 0;
    obj->field_20 = 0;
    obj->y_scale = 0;
    obj->field_0c = 0;
    obj->accel_x = 0;
    obj->state_0++;
    if (obj->variant_index != 0) {
        obj->pos_x = 0x228;
        obj->health = 0x128;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    o = obj->parent;
    ref_other.p = o;
    init_hud_slot_for_other_side_slot0b(obj, o);
}

void init_hud_slot_for_other_side_slot0b(Object *obj, Object *other) {
    HudSlot *s = &data_801e7648_slot0b[other->side];
    s->init_size_a = 0x10;
    s->init_size_b = 0x10;
    s->field_0a = 2;
    s->slot_type = 0;
    s->init_const_byte = 0x18;
    s->data_ptr = data_801e4cfc_slot0b[other->kind];
    set_hud_slot_position_from_object_slot0b(obj, other);
}
