/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void move_cam_right(Cam *cam, short step)
{
  if (step >= 5)
  {
    step = 5;
  }
  cam->pos_x += step;
  set_cam_pos_x(cam, (short) ((short) cam->pos_x < cam->max_x ? cam->pos_x : cam->max_x));
}
