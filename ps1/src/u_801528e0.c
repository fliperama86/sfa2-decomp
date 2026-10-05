/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual (32 slots): same instruction count. The original keeps object in
   a0, builds the row/column sum in its own register (a1) and copies it into
   the index register (v1); built moves object to a1 and folds the sum into the
   index. Tried: ternary or if/else for the index, local temporaries for the
   row and column, operand order, declaration order, local copies of side and
   ref_other.p. */
/* Form found by automatic permutation search. */
void func_801528e0(Object *object)
{
  int idx;
  int base;
  ref_other.p = object->field_3c;
  idx = object->field_03 >> 1;
  base = ((ref_other.p->frame->field_0a & 0x1f) << 3) + idx;
  idx = (1 & game_state.field_1d) ? (base + 4) : (base);
  if (ref_other.p->side == 0)
  {
    ref_first.p = *((Object **) 0x1f800084);
  }
  else
  {
    ref_first.p = *((Object **) 0x1f800134);
  }
  base = ((u16 *) ref_first.p)[idx];
  object->field_0b = ref_other.p->field_0b;
  if (object->field_0b != 0)
  {
    base = -base;
  }
  object->pos_x = base + ref_other.p->pos_x;
  base = ((u16 *) ref_first.p)[idx + 1];
  object->pos_y = ref_other.p->pos_y - base;
  object->field_0b ^= 1;
}
