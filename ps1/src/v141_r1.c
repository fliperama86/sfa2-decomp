/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8013ae00(Object *a, Object *b, Tx *c)
{
  u8 value = c->field_13;
  s32 mask;
  if (b->field_61 == 0xff)
  {
    value >>= 1;
  }
  if (a->field_65)
  {
    ref_first.p = &player_right;
  }
  else
  {
    ref_first.p = &player_left;
  }
  if (ref_first.p->field_246 == 0)
  {
    func_80141f28(ref_first.p, value);
  }
  if (a->field_65)
  {
    ref_first.p = &player_left;
  }
  else
  {
    ref_first.p = &player_right;
  }
  mask = (-(b->field_61 != 0xff)) & 3;
  func_80141f28(ref_first.p, mask);
  if (b->field_7e != 0)
  {
    func_80141f28(ref_first.p, mask - 0x30);
  }
}
