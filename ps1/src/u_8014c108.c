/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Form found by automatic permutation search. */
void func_8014c108(Object *object)
{
  u16 v = *(data_80189460++);
  Object *other = object->other;
  u16 new_var;
  Object *new_var2;
  new_var2 = other;
  v = (u16) (other->ground_y - v);
  new_var = v;
  if (new_var <= ((u16) new_var2->pos_y))
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
