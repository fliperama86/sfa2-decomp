/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801e4710_slot0b[])(ModObj *);
extern void (*data_801e471c_slot0b[])(ModObj *);
extern u8 *data_801e46b4_slot0b[];
void set_mode_and_child_mode_slot0b(ModObj *obj);
void build_scaled_rotated_quads_from_layout_slot0b(ModObj *obj, void *a, int b, int c);

void call_handler_by_state_index_slot0b(Object *o) {
    ModObj *obj = (ModObj *)o;
    data_801e4710_slot0b[obj->state_index](obj);
}

void nop_mod_object_handler_slot0b(ModObj *obj) {
}

void update_owned_object_by_substate_slot0b(ModObj *obj) {
    ref_other.p = obj->owner_object;
    if (data_80190468.p->state_0 == 0) {
        data_801e471c_slot0b[obj->substate_index](obj);
        if (data_80190a40 != 1) {
            build_scaled_rotated_quads_from_layout_slot0b(obj, data_801e46b4_slot0b[ref_other.p->side], ref_other.p->side, data_801a27d0);
        }
    } else {
        set_mode_and_child_mode_slot0b(obj);
    }
}

void set_mode_and_child_mode_slot0b(ModObj *obj) {
    ModObj *child = obj->child;
    obj->mode = 3;
    if (child != 0) {
        child->mode = 2;
    }
}
