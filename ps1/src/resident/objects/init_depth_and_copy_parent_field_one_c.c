/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_depth_and_copy_parent_field_one_c(Object *object) {
  object->ot_depth = 4;
  object->field_0f = 1;
  object->state_0++;
  ref_other.p = object->parent;
  object->field_1c = ref_other.p->field_1c;
}
