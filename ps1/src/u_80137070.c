/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Form found by automatic permutation search. */
void store_image_rect_e0_1e0_slot_2(void)
{
  Rect r;
  u8 *p = data_801a27e4 + 0x800;
  r.x = 0xe0;
  r.y = 0x1e0;
  r.w = 0x10;
  r.h = 0x20;
  func_80158028(&r, p);
  p = 0x1400 + p;
  func_80158028(&r, p);
  data_801aa4dc[2] = 0x1f;
}
