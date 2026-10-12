/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern s16 data_80026a08_slot27[];
void load_palette_anim_from_table_index_slot27(Object *obj);

void init_two_child_objects_and_set_motion_slot27(Object *obj) {
    s32 *p0;
    s32 *p1;
    s32 *p2;
    s32 *p3;

    obj->state_2++;
    obj->countdown = 0x258;
    ref_other.p->field_d8 = 0;
    obj->active_state = 0xff;
    ref_second.p = ref_other.p;
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count++;
        ref_other.p->field_02 = 0x22;
        ref_other.p->table_index = 0;
        ref_other.p->other = ref_second.p;
        ref_other.p->parent = obj;
        ((ObjectWords *)obj)->word_54 = (s32)ref_other.p;
    }
    ref_other.p = (Object *)pop_block_from_pool();
    if (ref_other.p != 0) {
        ref_other.p->active_count++;
        ref_other.p->field_02 = 0x22;
        ref_other.p->table_index = 1;
        ref_other.p->other = ref_second.p;
        ref_other.p->parent = obj;
        ((ObjectWords *)obj)->word_58 = (s32)ref_other.p;
    }
    p0 = (s32 *)&data_8019045c;
    p1 = data_80190464;
    p2 = data_8019046c;
    p3 = (s32 *)&data_8019047c;
    *p0 = 0;
    *p1 = -2;
    *p2 = 0;
    ref_other.p = ref_second.p;
    *p3 = 1;
    if (ref_other.p->field_d8 != 0) {
        *p0 = 0x10;
        *p1 = 2;
        *p2 = 1;
        *p3 = 0;
    }
    obj->health = *p0;
    obj->y_offset = *p1;
    ref_first.p = (Object *)((ObjectWords *)obj)->word_54;
    ref_second.p = (Object *)((ObjectWords *)obj)->word_58;
    ref_first.p->field_62 = *p2;
    ref_second.p->field_62 = *p3;
    *p0 = *p0 / 16;
    obj->field_62 = *p0;
    obj->vec_table_index = *p0;
    *p0 = 2;
    *p0 = ref_other.p->field_d8 + 2;
    load_palette_anim_from_table_index_slot27(obj);
    *p0 = ref_other.p->side * 2;
    data_80190464[0] = data_80026a08_slot27[data_8019045c[0]];
    data_8019046c[0] = data_80026a08_slot27[data_8019045c[0] + 1];
    obj->vel_x = data_80190464[0];
    obj->vel_y = data_8019046c[0];
    data_80190464[0] = data_80190464[0] << 3;
    data_8019046c[0] = data_8019046c[0] - data_80190464[0];
    obj->pos_x = data_8019046c[0];
    obj->pos_y = 0x3c;
}
