/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_801e766c_slot0b[];
void build_sprite_set_quads_into_ot_slot0b(Object *obj, u8 *a);

void follow_ref_object_and_build_quads_slot0b(Object *obj) {
    obj->pos_x = ref_other.p->pos_x + ((u16 *)&obj->vel_x)[1];
    obj->pos_y = ref_other.p->pos_y + ((u16 *)&obj->vel_x)[1];
    if (obj->table_index == 0 && data_80190a40 != 1) {
        build_sprite_set_quads_into_ot_slot0b(obj, data_801e766c_slot0b + obj->variant_index * 0x1e0 + draw_buffer_index * 0xf0);
    }
}

void unlink_from_parent_list_and_release_slot0b(Object *obj) {
    Object **p = (Object **)obj->parent->linked_object;
    int n;

    for (n = 4; n > 0; n--) {
        if (obj == *p) {
            *p = 0;
            break;
        }
        p++;
    }
    clear_object_and_push_on_free_stack(obj);
}

void load_velocities_from_table_slot0b(Object *obj) {
    unlink_from_parent_list_and_release_slot0b(obj);
}
