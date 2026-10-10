/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the resident executable; the build does not use this
 * file. The differential test next to it (difftest.py, with func_8013b0c4.py)
 * compares the behavior of this C with the original code on random inputs
 * of the contract below.
 *
 * What it does (inferred, not an original name): `object` is the object
 * that triggered a hit, `other` the object that was hit, `g` the Box32 record that the callers pass (inferred; the role is not known).
 * If the pool of spawnable objects (data_801a27d4, a byte counter) is not 0
 * and the pool hands out an object, it fills that object in as a hit effect
 * (kind 4, copies of fields of object and other, a link back to other,
 * a rectangle size, a draw hook, and a position at the centre of the
 * rectangle data_80188ed0) and counts the pool down. If other's field_61
 * has bit 0x80 the effect's field_48 is 1. Otherwise, when g->field_17 has
 * bit 0x80 and the pool is still not empty, a second effect is made the
 * same way, without the position and without masking field_03. Afterwards,
 * if other's field_61 has bit 0x80 the function starts an animation on
 * other (func_80120554, or func_801204f4 when other's kind is 6) from the
 * tables at 0x80177390; else, unless g->field_10 has bit 0x80, it starts
 * one chosen by an index from g->field_10 (doubled, with bit 0 set when
 * other's field_5c is negative) and then a follow-up step from a table by
 * other's field_66.
 *
 * Contract (the roles named for the fields are inferred):
 *   Arguments: a0 = object, a1 = other, a2 = g (Box32 *, as in the prototype; its field_10 and field_17 are read). No return value.
 *   Reads: ref_other (a word; kept in data_80188f20 and put back),
 *     data_801a27d4, g's
 *     field_10 and field_17, object's field_65, other's field_72, field_0e,
 *     field_61, field_5c, kind, side and field_66, the rectangle
 *     data_80188ed0 (out_3c, out_40, out_44, out_48), the tables
 *     table_8017736c (16 halfwords from 0), table_80177390 (one halfword
 *     per kind) and table_80197ef8 (one halfword per field_66 value).
 *   Writes: data_80188f20 (the old value of ref_other, always),
 *     data_801a27d4 (decremented once per effect), the effect
 *     object(s) (fields 00, 02, 03, 0b, 0e, 3c, 48, 65, 76 to 7c, 90,
 *     pos_x, pos_y in the first), other's field_17f (first effect only).
 *     ref_other is set to the pool's result in between and put back at the
 *     end, so it is unchanged when the function returns.
 *   Callees replaced by recorders (the same in the original and this C):
 *     func_8011f1e0 (0 arguments) returns the effect object, or 0 in one
 *     case of five; because it is a recorder it returns the same value
 *     both times it is called, so the second effect overwrites the first
 *     when both are made. func_80120554 and func_801204f4 (3 arguments),
 *     func_80120444 (2 arguments) return 0; the function ignores their
 *     results. The second argument of func_80120444 is a byte that the
 *     callee masks; the original passes a sign-extended halfword and this C
 *     the byte, so the test logs only its low byte.
 *   Watched at every recorded call (copied into the log, so the order of
 *     the function's stores against the calls is tested): other, the first
 *     0x100 bytes of the effect object, data_801a27d4, data_80188f20 and
 *     ref_other. No recorded callee gets a pointer to memory filled for it.
 *   Aliasing: object, other, g, the effect object and the tables are
 *     distinct blocks. other's field_66 is limited to the size of the
 *     table_80197ef8 block the setup makes; the index from g->field_10 is
 *     0 to 255 and so reaches table_8017736c beyond 16 entries: the setup
 *     gives that table block room for them.
 *   Not reached by any input: see the coverage line of the run.
 * The tree declares func_8011f1e0 as returning a Block172 pointer; the result is cast to Object * here.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80188f20;
extern s16 table_8017736c[];
extern s16 table_80177390[];
extern s16 table_80197ef8[];

void func_8013b0c4(Object *object, Object *other, Box32 *g)
{
  Object *fx;
  int centre_x;
  int centre_y;
  s16 index;
  u8 k;
  s16 step;

  data_80188f20 = ref_other.p;
  if ((data_801a27d4 != 0) && ((fx = ref_other.p = (Object *)func_8011f1e0()) != 0))
  {
    fx->field_00++;
    fx->field_02 = 4;
    fx->field_65 = object->field_65;
    fx->field_0b = other->field_72;
    fx->field_0e = other->field_0e;
    fx->field_3c = other;
    fx->field_03 = g->field_17 & 0x7f;
    other->field_17f = g->field_17 & 0x7f;
    fx->field_76 = 0x240;
    fx->field_78 = 0;
    fx->field_7a = 0x60;
    fx->field_7c = 0x1e0;
    fx->field_90 = (void *) 0x800fb100;
    data_801a27d4--;
    centre_x = ((s16) data_80188ed0.out_3c - (s16) data_80188ed0.out_40) >> 1;
    fx->pos_x = centre_x + (s16) data_80188ed0.out_40;
    centre_y = ((s16) data_80188ed0.out_44 - (s16) data_80188ed0.out_48) >> 1;
    fx->pos_y = centre_y + (s16) data_80188ed0.out_48;
    fx->field_48 = 0;
    if (other->field_61 & 0x80)
    {
      fx->field_48 = 1;
    }
    else if (((g->field_17 & 0x80) && (data_801a27d4 != 0)) && ((fx = ref_other.p = (Object *)func_8011f1e0()) != 0))
    {
      fx->field_00++;
      fx->field_02 = 4;
      fx->field_03 = g->field_17;
      fx->field_65 = object->field_65;
      fx->field_0b = other->field_72;
      fx->field_0e = other->field_0e;
      fx->field_3c = other;
      fx->field_48 = 0;
      fx->field_76 = 0x240;
      fx->field_78 = 0;
      fx->field_7a = 0x60;
      fx->field_7c = 0x1e0;
      data_801a27d4--;
      fx->field_90 = (void *) 0x800fb100;
    }
  }
  ref_other.p = data_80188f20;
  if (other->field_61 & 0x80)
  {
    if (other->kind != 6)
    {
      func_80120554(other, other->field_66, table_80177390[other->kind]);
    }
    else
    {
      func_801204f4(other, other->field_66, table_80177390[6]);
    }
  }
  else if (!(g->field_10 & 0x80))
  {
    index = g->field_10 * 2;
    if ((s16) other->field_5c < 0)
    {
      index |= 1;
    }
    if (index >= 16)
    {
      func_80120554(other, other->side, 0x32e);
      func_801204f4(other, other->side ^ 1, table_8017736c[index]);
    }
    else
    {
      func_80120554(other, other->side, table_8017736c[index]);
    }
    k = other->field_66;
    step = table_80197ef8[k];
    if (step != 3)
    {
      func_80120444(k, step);
    }
  }
}
