/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8012da2c(Object *object)
{
  int v;
  int w;
  int t;
  unsigned short new_var;
  u8 index;
  if (((s16) object->step_flags) >= 0)
  {
    func_80130efc(object);
    return;
  }
  if (object->field_163 == 0)
  {
    object->state_0 = 1;
    object->state_1 = 1;
    object->state_2 = 2;
    object->state_3 = 0;
    object->field_15b = 0;
    object->field_247 = 0;
    if ((game_state.field_30 != 0) && (game_state.mode != (object->side + 1)))
    {
      object->field_1a2 = 0;
    }
    return;
  }
  w = object->field_162;
  object->state_0 = 1;
  object->state_1 = 0;
  object->state_2 = 9;
  object->state_3 = 0;
  object->field_15d = 0;
  object->field_15e = 0;
  object->field_163 = 0;
  object->field_162 = w + 5;
  func_80155f30(object);
  object->field_69 = 0;
  object->field_6a = 0;
  if ((game_state.field_30 != 0) && (game_state.mode != (object->side + 1)))
  {
    v = object->field_1a1 + 0xff;
    object->field_1a1 = v;
    if (v & 0x80)
    {
      object->field_19f = 0;
    }
  }
  object->field_170 = 0;
  object->field_157 = 0;
  t = func_80151184() & 0x1e;
  index = t;
  new_var = index;
  func_801204f4(object, object->side, 2);
  if (object->kind == 9)
  {
    object->field_46 = table_80171194[new_var];
  }
  else
  {
    object->field_46 = table_80171174[new_var];
  }
  func_801308c4(object, 0x1a);
}
