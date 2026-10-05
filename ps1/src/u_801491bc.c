/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011a55c(void);

/* Form found by automatic permutation search. */
void func_801491bc(Object *object)
{
  Object *other = object->field_3c;
  int index;
  int w;
  int d;
  Object *new_var;
  func_8011f240(object);
  d = other->field_02;
  new_var = other;
  if (d == 0)
  {
    index = 0x10;
    d = player_left.field_d4;
    w = 0x100;
  }
  else
  {
    index = 0xb;
    d = player_right.field_d4;
    w = 0x120;
  }
  func_80136f10(new_var, index, w, (5 * d) + 0x1e0);
  func_80137220(0, 6);
}
