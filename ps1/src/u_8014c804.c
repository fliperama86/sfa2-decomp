/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: a1/a2 swapped for the two loaded halfwords. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014c804(Object *object)
{
  unsigned short new_var;
  u16 bits;
  u16 limit;
  if (object->script_arg == 0)
  {
    limit = *(data_80189460++);
    bits = *(data_80189460++);
    if (((s16) limit) < object->table_column)
    {
      object->script_bits |= bits;
    }
  }
  else
    if (object->script_arg == 2)
  {
    limit = *(data_80189460++);
    bits = *(data_80189460++);
    new_var = ~bits;
    if (((s16) limit) < object->table_column)
    {
      object->script_bits &= new_var;
    }
  }
  else
    if (object->script_arg == 4)
  {
    object->script_counter = 0;
  }
  else
    if (object->script_arg == 6)
  {
    object->script_counter++;
  }
  func_8014c914(object);
}
