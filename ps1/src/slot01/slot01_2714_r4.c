/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800319b8_slot01[];
void build_sized_sprite_quads_into_ot_slot01(Object *obj, u8 *a);

void update_or_draw_sprite_set_slot01(Object *obj) {
    if (obj->table_index != 0) {
        advance_sequence_checked(obj);
        set_flag_and_push_if_mode_8(obj);
    } else {
        build_sized_sprite_quads_into_ot_slot01(obj, data_800319b8_slot01 + data_801a27d0 * 0xf0);
    }
}

void unlink_from_parent_list_and_release_slot01(Object *obj) {
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
