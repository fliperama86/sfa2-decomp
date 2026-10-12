/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void branch_script_if_value_ge_table_column(Object *object)
{
  short v = *(data_80189460++);
  u16 new_var;
  new_var = v >> 8;
  object->script_byte_hi = new_var;
  object->script_byte_lo = v;
  if ((v & 0x1f) >= object->table_column)
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
