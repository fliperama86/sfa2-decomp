/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8012bbac(Object *object)
{
  object->field_0b = object->field_158;
  ref_other.p = object->other;
  if (ref_other.p->field_157 == 0)
  {
    object->field_07 = 0;
    func_80130678(object, 0x15);
    return;
  }
  if (ref_other.p->field_45 != 0)
  {
    object->field_07 = 0;
    func_80130678(object, 0x15);
    return;
  }
  object->field_07 = 2;
  func_80130678(object, 0x18);
}
