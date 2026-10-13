/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_object_at_side_position_from_parent_meter(Object *object) {
  u8 v;
  object->pos_x = 0x20;
  object->pos_y = 0xd2;
  object->countdown = 0x1e;
  object->sequence = data_8017cf60;
  object->field_0c = 0;
  object->ot_depth = 0;
  object->field_20 = 0;
  object->table_index_2 = 0;
  object->state_0++;
  ref_other.p = object->parent;
  if (ref_other.p->side != 0) {
    object->pos_x += 0xb4;
  }
  v = *(u8 *) &ref_other.p->meter + 0x1e;
  if (object->field_4b != 0) {
    v = 0x48;
  }
  object->variant_index_2 = v;
}

void update_variant_index_from_parent_meter(Object *object) {
  Object *other;
  u8 v;
  u8 a;
  u8 b;
  int t;
  other = ref_game_state.p;
  b = ((u8 *) other)[0x4d] | ((u8 *) other)[0x4e];
  a = other->state_0 | single_object_active;
  if (a | b) {
    set_state_0_to_end_clear_flag_byte(object);
  } else {
    ref_other.p = object->parent;
    v = *(u8 *) &ref_other.p->meter + 0x1e;
    if (ref_other.p->field_4b != 0) {
      v = 0x48;
    }
    t = object->countdown;
    t -= 1;
    object->countdown = t;
    if ((s16) t < 0) {
      v = ref_other.p->cursor_timer_a;
      if (v == 0) {
        object->state_0++;
      }
    }
    object->variant_index_2 = v;
    set_flag_and_push_if_mode_8(object);
  }
}
