/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Written by hand in the form of move_cam_right, which was finished by hand from a candidate that the automatic permutation search had reshaped. */
void move_cam_left(Cam *cam, short step)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  int m;
  step = -step;
  if (step >= 5)
  {
    step = 5;
  }
  m = cam->min_x;
  cam->pos_x -= step;
  set_cam_pos_x(cam, (short) ((short) cam->pos_x > m ? cam->pos_x : m));
}
