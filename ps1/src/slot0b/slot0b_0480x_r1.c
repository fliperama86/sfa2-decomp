/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801e4614_slot0b[];
void queue_pooled_clut_upload_job_b_slot0b(Object *obj);
void set_sequence_from_slot01_table_slot0b(Object *obj, u8 a);

void init_object_from_parent_kind_slot0b(Object *obj) {
    u8 k;

    ref_other.p = obj->parent;
    obj->pos_x = 0x48;
    obj->countdown = 0xf;
    obj->health = 0x48;
    obj->pos_y = 0x68;
    obj->y_offset = 0x88;
    obj->vel_x = 0;
    obj->accel_x = 0;
    obj->vel_y = 0;
    obj->accel_y = 0;
    obj->facing_side = obj->variant_index;
    if (obj->variant_index != 0) {
        obj->pos_x = 0x128;
        obj->health = 0x128;
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    k = ref_other.p->kind;
    if (k == 0x14) {
        queue_pooled_clut_upload_job_b_slot0b(obj);
    } else {
        get_byte_from_table_by_index(k);
    }
    set_sequence_from_slot01_table_slot0b(obj, get_byte_from_table_by_index(ref_other.p->kind));
}
