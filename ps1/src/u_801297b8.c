/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Not exact: 2 slots differ. In the second block the original loads the
   constant 1 (ori v1) into the branch delay slot and then the constant 2
   (ori v0); this build emits them in the opposite order. Source reorderings
   of the stores moved the stores but not the two loads. */
void func_801297b8(Object *object)
{
  int t;
  int new_var;
  t = object->field_290;
  object->field_5c = object->field_5c - t;
  if (((s16) object->field_5c) < 0)
  {
    object->state_0 = 1;
    object->state_1 = 1;
    new_var = 0;
    object->state_2 = new_var;
    object->state_3 = 0;
    t = 2;
    object->field_62 = t;
    object->field_61 = 7;
    object->field_15b = 1;
    game_state.field_358->field_167 = object->field_28e;
    if (object->field_73 != 0)
    {
      game_state.field_358->state_0 = 1;
      game_state.field_358->state_1 = 1;
      game_state.field_358->state_2 = 1;
      game_state.field_358->state_3 = 0;
      game_state.field_358->field_63 = 0;
      game_state.field_358->field_60 = t;
      game_state.field_358->field_61 = 5;
      object->field_73 = 0;
      game_state.field_358->field_73 = 0;
      game_state.field_358->field_261 = 0;
      game_state.field_358->field_72 = object->field_0b;
      game_state.field_358->field_72 = game_state.field_358->field_72 ^ 1;
      game_state.field_358->field_243 = 1;
    }
    t = object->field_28e;
    t = (t - 12) * 2;
    game_state.field_358->field_255 = t;
    game_state.field_6b = 0;
    func_80147000(object);
  }
  object->field_28c = object->field_28c - 1;
  if (object->field_28c & 0x80)
  {
    object->field_28c = 0;
    object->field_28d = 0;
    object->field_297 = 0;
    object->state_0 = 1;
    object->field_62 = 2;
    object->state_1 = 1;
    object->state_2 = 0;
    object->state_3 = 0;
    object->field_61 = 3;
    object->field_15b = 1;
    object->field_163 = 1;
    object->field_15d = 0;
    object->field_15e = 0;
    object->field_72 = game_state.field_358->field_0b;
    object->field_63 = 0;
    object->field_01 = 1;
    func_801204f4(object, object->side ^ 1, 0xf);
  }
  object->field_28d = object->field_297;
}
