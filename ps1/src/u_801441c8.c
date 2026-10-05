/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




/* Form found by automatic permutation search. */
int func_801441c8(void)
{
  Object *p = ((ConfigHead *) game_state.config)->field_78;
  int result = 0;
  long new_var;
  if (p != 0)
  {
    ref_first.p = p;
    new_var = ref_first.p->field_cd;
    if (0 == new_var)
    {
      result = (s16) (new_var = ref_first.p->field_5c);
      result = result == 0x90;
    }
  }
  return result;
}
