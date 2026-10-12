/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Form found by automatic permutation search. */
void format_object_score_text(Object *o)
{
  u8 d[7];
  int i;
  int new_var3;
  int new_var2;
  u8 *q;
  TextObj *new_var;
  unsigned v;
  u8 nz;
  TextObj *t;
  if ((game_state.versus_flag == 0) && (game_state.mode3_active_flag == 0))
  {
    v = o->score;
    i = 0;
    do
    {
      d[i] = v & 0xf;
      v >>= 4;
      i++;
    }
    while (i < 7);
    nz = 0;
    new_var = table_80180f1c[o->side];
    i = 0;
    new_var2 = 0x20;
    q = d + 6;
    t = new_var;
    do
    {
      u8 c = *q;
      new_var3 = 0;
      if (v = c != new_var3)
      {
        nz = 1;
      }
      if (nz)
      {
        t->buf[i] = c + 0x30;
      }
      else
      {
        t->buf[i] = new_var2;
      }
      i++;
      q--;
    }
    while (i < 6);
    t->buf[i] = d[6 - i] + 0x30;
    add_item_to_table(t);
  }
}
