/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void func_80155de0(Actor *a, Object *b)
{
  Object *o = &player_left;
  s8 side;
  u8 idx;
  int new_var;
  if (ref_third.p->team_id)
  {
    o++;
  }
  if ((b->hit_type < 0x80) && (o->mode_flag_2 == 0))
  {
    side = o->team_id;
    new_var = a->text_index & 0x1f;
    o->field_bf = 0xff;
    idx = new_var;
    o->sound_index = idx;
    func_80155d4c(table_80181510[idx], side);
    func_80155eac(table_80181510[a->text_index & 0x1f], side);
  }
}
