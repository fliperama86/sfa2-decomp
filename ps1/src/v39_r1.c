/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void update_round_winner(Attacker *a, int side)
{
  Object *p;
  Object *q;
  Object *t;
  Object *new_var2;
  u8 n;
  short new_var;
  u8 new_var3;
  p = &player_left;
  q = p + 1;
  new_var2 = ref_first.p = p;
  if (((u8) side) == 2)
  {
    t = new_var2;
    p = q;
    q = t;
    ref_first.p = p;
  }
  a->winner = p;
  a->loser = q;
  if (q->mode_flag_2 == 0)
  {
    q->db_flag = 0xff;
  }
  p->win_count++;
  n = a->win_target;
  if (p->win_count == n)
  {
    a->field_45 = 0xff;
    a->field_60 = 0;
    n = n - 1;
    new_var = (new_var3 = (u8) n);
    if (a->round_count == new_var)
    {
      a->field_60 = 0xff;
      a->field_77++;
    }
    if (a->field_1b == 3)
    {
      u8 v = p->win_streak;
      if (v < 99)
      {
        v++;
      }
      p->win_streak = v;
      p->score_sort_key = v;
      q->win_streak = 0;
      if (a->field_2f != 0)
      {
        table_801aa4e8[0][0][(p->side * 42) + p->kind]++;
        table_801aa4e8[q->side][1][q->kind]++;
        data_801aa53c[p->side]++;
      }
    }
    else
      if (p->mode_flag_2 == 0)
    {
      p->saved_health = p->health;
      p->field_fe = p->meter;
      if ((a->mode_marker == 0) && (a->field_54 == a->compare_value))
      {
        a->field_90 = 0xff;
        a->field_2c = 0xff;
      }
    }
  }
}
