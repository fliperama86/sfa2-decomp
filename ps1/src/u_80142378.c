/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



/* Form found by automatic permutation search. */
u8 func_80142378(Object *object)
{
  SequenceStep *step;
  FrameRecord *frame;
  Box32 *boxes;
  s16 elapsed;
  game_state.other_object = object->other;
  if ((((u32 *) game_state.other_object)[1] & 0xffffff) != 0x50001)
  {
    return 0;
  }
  elapsed = 0;
  step = game_state.other_object->sequence;
  frame = game_state.other_object->frame;
  boxes = game_state.other_object->wide_boxes;
  for (;;)
  {
    if (frame->active)
    {
      boxes = boxes + frame->active;
      return func_80142424(object, boxes, elapsed);
    }
    elapsed += step->duration;
    if (step->flags < 0)
    {
      return 0;
    }
    ++step;
  }

}
