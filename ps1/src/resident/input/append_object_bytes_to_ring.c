/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Form found by automatic permutation search. */
void append_object_bytes_to_ring(GameState *g)
{
  Object *o;
  int i;
  short n;
  u16 head;
  if (g->mode != 3)
  {
    o = g->focus_object;
    if (o->mode_flag_2 == 0)
    {
      n = o->win_count;
      if (o->team_id == 0)
      {
        head = g->head_a;
        for (i = 0; i < n; i++)
        {
          g->ring_a[(s16) head] = o->bytes_d0[i];
          head = (head + 1) & 0x3f;
        }

        g->head_a = head;
      }
      else
      {
        head = g->head_b;
        for (i = 0; i < n; i++)
        {
          g->ring_b[(s16) head] = o->bytes_d0[i];
          head = (head + 1) & 0x3f;
        }

        g->head_b = head;
      }
    }
  }
}
