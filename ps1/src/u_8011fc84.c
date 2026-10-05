/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
void func_8011fc84(Object *object, SeqRec *rec)
{
  int w;
  int new_var;
  object->sequence = (SequenceStep *) rec;
  new_var = rec->header;
  w = new_var;
  new_var = w >> 16;
  object->field_38 = new_var;
  object->field_3a = w;
  func_8011fcc0(rec);
}
