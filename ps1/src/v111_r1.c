/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
/* The path for field_29f has its own call with the constant. The compiler
   merges only the call instruction. The index local has the callee's
   parameter type: with int the compiler drops the constant load of that
   path, because the argument register already holds the value. */
void func_8012d310(Object *object)
{
  u16 index;
  index = 0xc;
  object->field_157 = 0;
  if (object->field_29f != 0) { func_801308c4(object, 0xc); return; }
  ref_other.p = object->other;
  if ((ref_other.p->field_45 == 0) && (ref_other.p->field_157 != 0))
  {
    index = 0xe;
    object->field_157++;
  }
  func_801308c4(object, index);
}
