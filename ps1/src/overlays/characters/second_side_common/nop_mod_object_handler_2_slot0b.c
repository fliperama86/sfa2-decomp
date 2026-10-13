/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 *data_801e46b4_slot0b[];
void set_mode_and_child_mode_slot0b(ModObj *obj);
void build_scaled_rotated_quads_from_layout_slot0b(ModObj *obj, void *a, int b, int c);

void nop_mod_object_handler_2_slot0b(ModObj *obj) {
}

void build_owner_quads_unless_flag_set_mode_slot0b(Object *o) {
    Object *p = ((ModObj *)o)->owner_object;
    ModObj *obj = (ModObj *)o;
    if (single_object_active != 0) {
        set_mode_and_child_mode_slot0b(obj);
    } else {
        build_scaled_rotated_quads_from_layout_slot0b(obj, data_801e46b4_slot0b[ref_other.p->side], p->side, draw_buffer_index);
    }
}

void release_object_record_slot0b(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
