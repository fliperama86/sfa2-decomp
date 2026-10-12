/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80149920(Object *object) {
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

void func_801499c4(Object *object) {
  Object *other;
  u8 v;
  u8 a;
  u8 b;
  int t;
  other = data_80190468.p;
  b = ((u8 *) other)[0x4d] | ((u8 *) other)[0x4e];
  a = other->state_0 | data_801ae02c;
  if (a | b) {
    func_80149aa8(object);
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
    func_80120028(object);
  }
}
