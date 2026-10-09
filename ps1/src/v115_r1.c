/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80136898(Cam *cam, int delta);

/* Written by hand in the form of func_80136668, which was finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_801366d4(Cam *cam, short step)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  int m;
  step = -step;
  if (step >= 5)
  {
    step = 5;
  }
  m = cam->field_44;
  cam->field_22 -= step;
  func_80136898(cam, (short) ((short) cam->field_22 > m ? cam->field_22 : m));
}
