/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: the s16 copy, >> 8 into a u16 temporary form now gives the original
 * sll 16 / sra 24. Still missing: the original has a move of the loaded halfword
 * into a second register before the shifts (and stores that copy as the low byte);
 * 3 forms tried, the compiler propagates the copy away. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014de94(Object *object);

/* Form found by automatic permutation search. */
void func_8014c914(Object *object)
{
  u16 v = *(data_80189460++);
  s16 w;
  w = v;
  v = w >> 8;
  object->field_20e = v;
  object->field_20f = w;
  object->field_238 = (s32) data_80189460;
  w = object->field_20e;
  if (w != 0xff)
  {
    func_8014a5c0(object);
    return;
  }
  func_8014c994(object);
  func_8014a5c0(object);
}
