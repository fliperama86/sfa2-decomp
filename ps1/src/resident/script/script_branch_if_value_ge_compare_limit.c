/* Reconstruction. Names/roles inferred, not original symbols. */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: script_branch_if_value_ge_compare_limit, script_branch_if_value_le_compare_limit (and r2 branch_script_if_value_ge_table_column, branch_script_if_table_column_ge_value): the original computes
   script_byte_hi as sll 16 / sra 24 of the raw halfword; every form tried (>> 8, casts to s16/s8, int/unsigned/u16 locals,
   s16 pointer) folds to srl 8. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void script_branch_if_value_ge_compare_limit(Object *object)
{
  short new_var;
  short new_var2;
  u16 v = *(data_80189460++);
  new_var = v;
  new_var2 = new_var >> 8;
  object->script_byte_hi = new_var2;
  object->script_byte_lo = v;
  if (v >= object->compare_limit)
  {
    read_script_word_and_dispatch(object);
  }
  else
  {
    skip_script_block_then_read_opcode(object);
  }
}
