/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (2 slots): the original loads the table value into v0 and negates
   into a1 (negu a1,v0), then negates a1 again in the else path. Here the load
   goes straight into a1. Separate pseudos (neg = -f) make the compiler fold
   -(-f) away (168 bytes); only the in-place form keeps the second negu. */
void func_80142968(Object *object)
{
  Pair *t = table_8017ad5c;
  short neg = (*(table_8017ad5c + (object->field_12a >> 1))).first;
  s16 lim;
  s16 pos = *((s16 *) (&object->field_c6));
  neg = -neg;
  lim = neg;
  if (lim >= pos)
  {
    u8 v = 4;
    if (pos < 0x90)
    {
      v = (pos >= 0x60) << 1;
    }
    object->field_12a = v;
    object->field_255 = v;
    func_80141f28(object, (s16) t[object->field_12a >> 1].first);
  }
  else
  {
    object->field_255 = object->field_12a;
    func_80141f28(object, (s16) (-neg));
  }
}
