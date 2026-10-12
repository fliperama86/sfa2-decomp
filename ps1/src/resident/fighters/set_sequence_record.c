/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"



/* Form found by automatic permutation search. */
void set_sequence_record(Object *object, SeqRec *rec)
{
  int w;
  int new_var;
  object->sequence = (SequenceStep *) rec;
  new_var = rec->header;
  w = new_var;
  new_var = w >> 16;
  object->step_timer = new_var;
  object->step_flags = w;
  store_rect_entries_and_queue_kinds(rec);
}
