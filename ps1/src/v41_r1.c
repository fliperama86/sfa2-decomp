/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"
/* Exact. The count is read into a local before the subtraction so the load order matches. */


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80129678(Object *object)
{
  unsigned short new_var;
  int cnt;
  new_var = 0;
  game_state.field_358 = object->other;
  if (((((((game_state.config->field_4d | game_state.config->field_4e) | game_state.config->field_04) == new_var) && ((*((u16 *) (&object->field_04))) != 0x301)) && ((*((u16 *) (&object->field_04))) != 0x201)) && (object->field_165 == new_var)) && ((object->field_28d & 0x80) == new_var))
  {
    object->field_28d--;
    if (object->field_28d == new_var)
    {
      func_801297b8(object);
      return;
    }
    if (object->field_28c == new_var && (cnt = object->field_28d, (object->field_297 - cnt) >= 0x18))
    {
      object->field_28c = new_var;
      object->field_28d = new_var;
      object->field_297 = new_var;
      object->field_04 = 1;
      object->field_05 = 1;
      object->field_06 = new_var;
      object->field_07 = new_var;
      object->field_62 = 2;
      object->field_61 = 3;
      object->field_15b = 1;
      object->field_163 = 1;
      object->field_15d = new_var;
      object->field_15e = new_var;
      object->field_72 = game_state.field_358->field_0b;
      object->field_63 = new_var;
      object->field_01 = 1;
      func_80120554(object, object->side ^ 1, 0x306);
    }
  }
}
