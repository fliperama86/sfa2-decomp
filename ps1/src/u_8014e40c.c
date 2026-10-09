/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

typedef u8 (*ScriptFn)(Object *object);
extern ScriptFn data_8017d340[];

/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: original stores data_80189468 right after the load and reuses
   the load register; here the store is scheduled after the table lookup. */
/* Form found by automatic permutation search. */
u8 func_8014e40c(Object *object)
{
  u16 v = *(data_80189460++);
  ScriptFn *new_var;
  data_80189468 = v;
  return (*(new_var = &data_8017d340[((s16) v) >> 9]))(object);
}
