/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"




/* Form found by automatic permutation search. */
int is_config_object_health_full(Object *unused)
{
  Object *p = ((ConfigHead *) game_state.config)->object;
  int result = 0;
  long new_var;
  if (p != 0)
  {
    ref_first.p = p;
    new_var = ref_first.p->mode_flag_2;
    if (0 == new_var)
    {
      result = (s16) (new_var = ref_first.p->health);
      result = result == 0x90;
    }
  }
  return result;
}
