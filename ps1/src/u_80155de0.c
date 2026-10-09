/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern ObjectRef ref_third;

/* Form found by automatic permutation search. */
void func_80155de0(Actor *a, Object *b)
{
  Object *o = &player_left;
  s8 side;
  u8 idx;
  int new_var;
  if (ref_third.p->field_65)
  {
    o++;
  }
  if ((b->field_61 < 0x80) && (o->field_cd == 0))
  {
    side = o->field_65;
    new_var = a->field_08 & 0x1f;
    o->field_bf = 0xff;
    idx = new_var;
    o->field_be = idx;
    func_80155d4c(table_80181510[idx], side);
    func_80155eac(table_80181510[a->field_08 & 0x1f], side);
  }
}
