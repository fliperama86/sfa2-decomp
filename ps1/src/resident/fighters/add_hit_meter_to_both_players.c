/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void add_hit_meter_to_both_players(Object *a, Object *b, Tx *c)
{
  u8 value = c->prim_code;
  s32 mask;
  if (b->hit_type == 0xff)
  {
    value >>= 1;
  }
  if (a->team_id)
  {
    ref_first.p = &player_right;
  }
  else
  {
    ref_first.p = &player_left;
  }
  if (ref_first.p->counter == 0)
  {
    add_to_meter_clamped(ref_first.p, value);
  }
  if (a->team_id)
  {
    ref_first.p = &player_left;
  }
  else
  {
    ref_first.p = &player_right;
  }
  mask = (-(b->hit_type != 0xff)) & 3;
  add_to_meter_clamped(ref_first.p, mask);
  if (b->field_7e != 0)
  {
    add_to_meter_clamped(ref_first.p, mask - 0x30);
  }
}
