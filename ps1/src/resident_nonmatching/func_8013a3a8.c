/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in size, instruction
 * scheduling and register choice. The exact owner of the bytes in the PS1
 * build stays the raw bytes of the resident executable; the build does not
 * use this file. The differential test next to it (difftest.py, with
 * func_8013a3a8.py) compares the behavior of this C with the original code
 * on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): applies one hit to the
 * fighter a1. a0 is the attacker, a2 the attack's record. A damage value
 * is built from tables (a base from the attacker's table, a random jitter
 * of one table row, minus 0x10), scaled by a mode that comes from a0 or a1
 * (x1, x2, x1.25, x1.5), capped at 0x7f and, for a1 in the state 0xff,
 * cut. Let X be: a1->field_d8 is 0 and (a0->field_08 is 8 or
 * a0->field_28a is 0). The cut is (dmg + 7) >> 3 (an eighth) when X and
 * a2->field_14 is not 0, or when not X and a2->field_14 is 0; it is
 * (dmg + 7) >> 2 (a quarter) when not X and a2->field_14 is not 0; and
 * the function returns with no effect at all when a2->field_14 is 0 and
 * (X or a1->field_5c is 0). A second stage (defence tables of a1,
 * a random term, a lookup in a damage table, a game option) turns it into
 * the points taken from a1->field_5c (a health value). A hit that cannot
 * be computed (the first sum is negative, or the cut leaves 0) only
 * removes 1 point, unless the option byte config->field_4c is not 0. On
 * the main path a health below 0 after the points are taken is first set to
 * -1 (with a1's field_163, field_15d and field_15e cleared), except that a1
 * with field_d8 not 0 in the state 0xff hit by a record with field_14 equal
 * to 0 is set to 0, which is no defeat. When the health is below 0 the
 * function records the defeat: it calls func_8013a154 (when data_80188f34 is
 * not 0) or func_8013ad64, writes a code into the attacking player's
 * field_167 (only when that is 0) and config->field_8f, and calls
 * func_80147000 (when a0->field_49 is not 0; otherwise when bit 0x40 of
 * a2->field_08 is set, config->field_4e is 0 and a2->field_16 is not 0). The
 * global pointers ref_first, ref_third and data_80188f24 are used as scratch
 * by the original and written here in the same order, because their last
 * values stay in memory (inferred).
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: a0 = attacker, a1 = target, a2 = attack record (Box32).
 *   No return value.
 *   Reads: fields of the three, of both players (player_left, player_right
 *     by a0->field_65), of game_state.config (field_12, field_4c and
 *     field_4d as one halfword in one place, field_4c in another, field_4e),
 *     data_80188f34, ref_other, and the tables table_801777d4,
 *     table_80177bd4, table_80177fd4, table_80178274, table_8017711c,
 *     table_8017721c, table_80178514, table_801787b4, table_80179834 and
 *     table_8017984c. The tables are read at indexes up to the ranges the
 *     setup fills with random bytes.
 *   Writes: a1->field_5c, field_15d, field_15e, field_163, field_260,
 *     field_28f; a player's field_167, field_255 and field_c6;
 *     config->field_6b and field_8f; ref_first, ref_third, data_80188f20,
 *     data_80188f24, data_80188f40, ref_other (put back), the random seed
 *     (data_80190126, by the callee func_80151184), and what the callee
 *     func_8013ad2c writes in a1 (inferred).
 *   Callees replaced by recorders, the same in both runs: func_8013ae00
 *     (3 arguments: a0, a1, a2; it reads them, the listing shows a0, a1
 *     and a2 live at its entry), func_8013a154 (3), func_8013ad64 (1),
 *     func_80147000 (1). All return 0 and the function ignores their
 *     results. func_80151184 (random byte) and func_8013ad2c run as the
 *     original code in both runs.
 *   Aliasing: a0, a1, a2, the config, the frame record and the players are
 *     distinct blocks.
 *   Excluded inputs: a0 and a1 are not the same object, and neither is a
 *     player object.
 *   Watched at every recorded call (copied into the log, so the order of
 *     the function's stores against the calls is tested): a1 (0x400 bytes),
 *     both players, the config block, ref_first, ref_third, ref_other,
 *     data_80188f20, data_80188f24, data_80188f40 and the seed word.
 *     No recorded callee gets a pointer to memory filled for that call.
 *   Not reached by any input: four instruction slots of the original, at
 *     offsets 0x1b4, 0x1f8, 0x230 and 0x270, each the correction for a
 *     negative value before a division by 4 of the damage; the damage is a
 *     byte masked to 8 bits and never negative.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80188f20;
extern Object *data_80188f24;
extern Object *data_80188f40;
extern u8 table_80177fd4[];
extern u8 table_80178274[];
extern u8 table_8017711c[];
extern u8 table_8017721c[];
extern u8 table_80178514[];
extern u8 table_801787b4[][128];

/* Not in protos.h (inferred from the listing and the callers' use). */
void func_8013ae00(Object *a, Object *b, Tx *c);
void func_8013ad2c(Object *object);
void func_8013ad64(Object *object);

/* The damage scale of mode 1, 2 or 3; any other mode leaves it as it is. */
static int scale_damage(int dmg, int mode)
{
  if (mode == 1)
  {
    return dmg << 1;
  }
  if (mode == 2)
  {
    return dmg + dmg / 4;
  }
  if (mode == 3)
  {
    return dmg + (dmg >> 1);
  }
  return dmg;
}

/* The defeat check at the end: when the target's health is below 0. */
static void check_defeat(Object *a0, Object *a1, Box32 *a2)
{
  int code;

  if ((s16) a1->field_5c >= 0)
  {
    return;
  }
  if (data_80188f34)
  {
    func_8013a154(a0, a1, a2);
  }
  else
  {
    func_8013ad64(a1);
  }
  ref_first.p = a0->field_65 ? &player_right : &player_left;
  if (ref_first.p->field_167 == 0)
  {
    code = ref_first.p->kind + 0x10;
    if (a0->field_49 == 0)
    {
      code = a2->field_15;
      if (data_80188f34)
      {
        code = (a2->field_08 & 0x40) ? 0xb : 7;
      }
    }
    ref_first.p->field_167 = code;
    game_state.config->field_8f = ref_first.p->kind;
  }
  if (a0->field_49)
  {
    data_80188f20 = ref_other.p;
    ref_first.p->field_255 = 6;
    ref_first.p->field_c6 = 0;
    game_state.config->field_6b = 0;
    func_80147000(a0);
    ref_other.p = data_80188f20;
  }
  else if (a2->field_08 & 0x40)
  {
    int step;

    if (game_state.config->field_4e != 0)
    {
      return;
    }
    if (a2->field_16 == 0)
    {
      return;
    }
    data_80188f20 = ref_other.p;
    if (a0->field_0b != 0)
    {
      game_state.config->field_6b = a2->field_16 - 1;
    }
    else
    {
      step = a2->field_16;
      if (step == 2)
      {
        step = step + 1;
      }
      else if (step == 3)
      {
        step = step - 1;
      }
      game_state.config->field_6b = step - 1;
    }
    func_80147000(a0);
    ref_other.p = data_80188f20;
  }
}

/* A hit that only takes one point (unless the option byte is set). */
static void graze(Object *a0, Object *a1, Box32 *a2)
{
  if (game_state.config->field_4c == 0)
  {
    a1->field_5c = a1->field_5c - 1;
  }
  check_defeat(a0, a1, a2);
}

void func_8013a3a8(Object *a0, Object *a1, Box32 *a2)
{
  int selector;
  int base;
  int row;
  int sum;
  int dmg;
  int mode;
  int cut;
  int armor;
  int shield;
  int row_offset;
  int kind_row;
  int points;
  int e;

  func_8013ae00(a0, a1, (Tx *)a2);
  ref_first.p = a0->field_65 ? &player_right : &player_left;
  data_80188f40 = ref_first.p;

  /* base damage of the attacker's table, then a random jitter of a row */
  selector = a0->field_49 ? a2->field_1a : a2->field_08;
  base = table_801777d4[ref_first.p->field_cf + ((selector & 0x1f) << 5)];
  row = a2->field_0e;
  if ((s16) a0->field_5c < 0x28)
  {
    row = a2->field_0f;
  }
  ref_third.p = data_80188f40;
  data_80188f24 = a0;
  ref_first.p = (Object *) &table_80177bd4[row * 32];
  ref_third.p = data_80188f24;
  sum = base + ((u8 *) ref_first.p)[func_80151184() & 0x1f] - 0x10;
  if (sum < 0)
  {
    graze(a0, a1, a2);
    return;
  }
  dmg = (u8) sum;

  /* the scale: from a1's mode, or a0's when both have one */
  if (a0->field_08 == 0)
  {
    if (a1->field_159 == 0)
    {
      mode = a1->field_29c;
    }
    else if (a0->field_29c == 0)
    {
      mode = a1->field_29c;
    }
    else if (!(a1->frame->field_0c & 0x80))
    {
      mode = a0->field_29c;
    }
    else
    {
      mode = a1->field_29c;
    }
    dmg = scale_damage(dmg, mode);
  }
  if (dmg >= 0x80)
  {
    dmg = 0x7f;
  }

  if (a1->field_61 == 0xff)
  {
    if (a1->field_d8 == 0 && (a0->field_08 == 8 || a0->field_28a == 0))
    {
      if (a2->field_14 == 0)
      {
        return;
      }
      cut = (dmg + 7) >> 3;
    }
    else if (a2->field_14 != 0)
    {
      cut = (dmg + 7) >> 2;
    }
    else
    {
      if (a1->field_5c == 0)
      {
        return;
      }
      cut = (dmg + 7) >> 3;
    }
    if (cut == 0)
    {
      graze(a0, a1, a2);
      return;
    }
    dmg = cut;
  }
  else
  {
    if (a2->field_15 == 5 || (a2->field_08 & 0x40))
    {
      a1->field_260 = 0xff;
    }
    mode = a2->field_0c;
    if (a0->field_49)
    {
      mode = 0;
      a1->field_15e = 0;
    }
    a1->field_15d = 0xd2;
    a1->field_15e = mode + a1->field_15e;
  }
  if (!(a1->field_15e < a1->field_162))
  {
    func_8013ad2c(a1);
  }

  /* defence of a1: its table value plus a term chosen by a2 */
  ref_first.p = (Object *) (a1->field_cd ? table_80178274 : table_80177fd4);
  kind_row = (a1->kind & 0x1f) << 5;
  armor = ((u8 *) ref_first.p)[a1->field_cf + kind_row];
  shield = 0x1f;
  if (a1->field_6a < 0x1f)
  {
    row_offset = 0;
    if (a1->field_295)
    {
      if (a2->field_1c)
      {
        row_offset = 0xe0;
        armor = 0;
      }
      else
      {
        row_offset = 0x80;
        if (a2->field_08 & 0x80)
        {
          row_offset = (a2->field_08 & 0x40) ? 0xc0 : 0xa0;
        }
      }
    }
    else if (a2->field_08 & 0x80)
    {
      row_offset = (a2->field_08 & 0x40) ? 0x40 : 0x20;
    }
    if (row_offset == 0x80)
    {
      row_offset = (a2->field_17 == 1) ? 0x20 : 0x40;
      ref_first.p = (Object *) table_8017721c;
    }
    else
    {
      ref_first.p = (Object *) table_8017711c;
    }
    shield = ((u8 *) ref_first.p)[a1->field_6a + row_offset];
  }
  e = armor + shield;
  if (a2->field_1b)
  {
    a1->field_28f = a2->field_1b;
  }
  if ((s16) a1->field_5c < 0x28)
  {
    e = table_80178514[kind_row + (func_80151184() & 0x1f)] + e;
  }
  if (e > 0x20)
  {
    e = 0x20;
  }
  e = table_801787b4[e][dmg];
  /* table_8017984c is declared as rows of 128 elsewhere; here it is read flat */
  e = e + ((u8 *) table_8017984c)[e + (table_80179834[a1->kind] >> 7)];

  /* the option byte scales the result */
  if (game_state.config->field_12 == 0)
  {
    e = e - (e >> 2);
  }
  else if (game_state.config->field_12 == 2)
  {
    e = e + (e >> 2);
  }
  else if (game_state.config->field_12 == 3)
  {
    e = e + (e >> 1);
  }
  if (e >= 0x80)
  {
    e = 0x7f;
  }

  /* config->field_4c and field_4d are read together as one halfword here */
  if ((game_state.config->field_4c | (game_state.config->field_4d << 8)) == 0)
  {
    points = e;
    a1->field_5c = a1->field_5c - points;
    if ((s16) a1->field_5c >= 0)
    {
      return;
    }
    if (a1->field_d8 != 0 && a1->field_61 == 0xff && a2->field_14 == 0)
    {
      a1->field_5c = 0;
    }
    else
    {
      a1->field_5c = -1;
      a1->field_163 = 0;
      a1->field_15d = 0;
      a1->field_15e = 0;
    }
  }
  check_defeat(a0, a1, a2);
}
