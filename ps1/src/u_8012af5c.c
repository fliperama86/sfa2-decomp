/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8012af5c(Object *object)
{
  s32 b;
  int new_var;
  s32 a;
  if ((object->step_flags << 16) >= 0)
  {
    update_sequence(object);
  }
  else
  {
    object->state_3 = 1;
    a = motion_recs[object->kind].vel_x;
    object->vel_y = motion_recs[object->kind].vel_y;
    b = motion_recs[object->kind].accel_x;
    object->accel_y = motion_recs[object->kind].accel_y;
    new_var = object->field_0b ^ 1;
    object->field_0b = new_var;
    if (object->field_0b != 0)
    {
      a = -a;
      b = -b;
    }
    object->vel_x = a;
    a = b;
    object->accel_x = a;
    func_80130678(object, 0x13);
  }
}
