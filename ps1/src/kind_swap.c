/* Reconstruction. Names/roles inferred, not original symbols. */
#include "object.h"

/* One routine of the character overlays in three placements. */
void overlay_shared_kind(Object *object);
void overlay_left_kind(Object *object);
void overlay_right_kind(Object *object);

void swap_kind(Object *object) {
    if (!(object->flags_28b & 1)) return;
    if (object->kind == 0x11) {
        object->kind = 0x13;
        if (object->other->kind == 0x11 || object->other->kind == 0x13) {
            overlay_shared_kind(object);
        } else if (object->side == 0) {
            overlay_left_kind(object);
        } else {
            overlay_right_kind(object);
        }
    } else if (object->kind == 0x13) {
        object->kind = 0x11;
        if (object->other->kind == 0x11 || object->other->kind == 0x13) {
            overlay_shared_kind(object);
        } else if (object->side == 0) {
            overlay_left_kind(object);
        } else {
            overlay_right_kind(object);
        }
    } else {
        return;
    }
    select_box_tables(object);
    build_metrics(object);
}
