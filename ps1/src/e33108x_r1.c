/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The copy j = index; stands after the first block that follows the call of
   func_801336a0: with the copy right after p = base + (index << 4); this
   function differs from the original in 41 instruction slots. The unused
   local reproduces stack space the original reserves: without it this
   function differs from the original in 14 instruction slots. The s16 copy j
   of the index stays: with every j replaced by index (and the copy removed)
   this function differs from the original in 6 instruction slots. The four
   pointers q are written (u8 *)((j << 4) + (int)base) rather than base + (j
   << 4): with the plain form in all four this function differs from the
   original in 4 instruction slots. */
void func_80133108(u8 *base, Object *left, Object *right)
{
  /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
  u8 unused[8];
  int index = *(s16 *)&data_801a27d0;
  s16 j;
  u8 *p;
  u16 c;
  s16 left_c6;
  s16 right_c6;
  data_80171c5c = 0;
  data_80171c5d = 4;
  data_80171c5e = 0;
  data_80171c5f = 0;
  data_80188d4c = 0;
  data_80188d50 = 0;
  data_80188d54 = 0;
  data_80188d58 = 0;
  data_80188d5c = 0;
  data_80188d60 = 0;
  p = base + (index << 4);
  *(u16 *)(p + 0xd0) = 0;
  *(u16 *)(p + 0xf0) = 0;
  *(u16 *)(p + 0x110) = 0;
  *(u16 *)(p + 0x130) = 0;
  *(u16 *)(p + 0x150) = 0;
  *(u16 *)(p + 0x170) = 0;
  func_801336a0(base, left, right, index);
  if (left->field_d8 != 0)
  {
    left->field_c6 = (s16)left->field_c6 * 3;
  }
  j = index;
  left_c6 = left->field_c6;
  if (left_c6 < 0x30)
  {
    *(s16 *)(p + 0xcc) = 0xa6 - left_c6;
    *(u16 *)(p + 0xd0) = left->field_c6;
  }
  else
  {
    if (left->field_d8 == 0)
    {
      p[0xc8] = 0x59;
      p[0xc9] = 0xd9;
      p[0xca] = 0xff;
      data_80171c5c = 1;
    }
    *(s16 *)(p + 0xcc) = 0x76;
    *(u16 *)(p + 0xd0) = 0x30;
  }
  c = left->field_c6;
  if ((unsigned)(c - 0x30) < 0x30)
  {
    *(s16 *)(base + (j << 4) + 0xec) = 0xa6 - c;
    *(s16 *)(base + (j << 4) + 0xf0) = left->field_c6 - 0x30;
  }
  else if (data_80171c5c == 1)
  {
    u8 *q = (u8 *)((j << 4) + (int)base);
    q[0xc8] = 0x20;
    q[0xc9] = 0xf0;
    q[0xca] = 0x20;
    q[0xe8] = 0x20;
    q[0xe9] = 0xf0;
    q[0xea] = 0x20;
    *(s16 *)(q + 0xec) = 0x46;
    *(s16 *)(q + 0xf0) = 0x30;
    data_80171c5c = 2;
  }
  else if (left->field_d8 != 0 && (s16)c >= 0x60)
  {
    *(s16 *)(base + (j << 4) + 0xec) = 0x46;
    *(s16 *)(base + (j << 4) + 0xf0) = 0x30;
  }
  c = left->field_c6;
  if ((unsigned)(c - 0x60) < 0x30)
  {
    *(s16 *)(base + (j << 4) + 0x10c) = 0xa6 - c;
    *(s16 *)(base + (j << 4) + 0x110) = left->field_c6 - 0x60;
  }
  else if (data_80171c5c == 2)
  {
    u8 *q = (u8 *)((j << 4) + (int)base);
    q[0xc8] = 0xf0;
    q[0xc9] = 0xf0;
    q[0xca] = 0x20;
    q[0xe8] = 0xf0;
    q[0xe9] = 0xf0;
    *(s16 *)(q + 0x10c) = 0x16;
    q[0xea] = 0x20;
    *(s16 *)(q + 0x110) = 0x30;
    data_80171c5c = 3;
  }
  else if (left->field_d8 != 0 && (s16)c >= 0x60)
  {
    *(s16 *)(base + (j << 4) + 0x10c) = 0x16;
    *(s16 *)(base + (j << 4) + 0x110) = 0x30;
    data_80171c5e = 1;
  }
  if (data_80171c5e != 0)
  {
    func_801340e4(base, j);
  }
  if (left->field_d8 != 0)
  {
    left->field_c6 = (s16)left->field_c6 / 3;
  }
  if (right->field_d8 != 0)
  {
    right->field_c6 = (s16)right->field_c6 * 3;
  }
  right_c6 = right->field_c6;
  if (right_c6 < 0x30)
  {
    *(s16 *)(base + (j << 4) + 0x130) = right_c6;
  }
  else
  {
    if (right->field_d8 == 0)
    {
      base[(j << 4) + 0x128] = 0x59;
      base[(j << 4) + 0x129] = 0xd9;
      base[(j << 4) + 0x12a] = 0xff;
      data_80171c5d = 5;
    }
    *(s16 *)(base + (j << 4) + 0x130) = 0x30;
  }
  c = right->field_c6;
  if ((unsigned)(c - 0x30) < 0x30)
  {
    *(s16 *)(base + (j << 4) + 0x150) = c - 0x30;
  }
  else if (data_80171c5d == 5)
  {
    u8 *q = (u8 *)((j << 4) + (int)base);
    q[0x128] = 0x20;
    q[0x129] = 0xf0;
    q[0x12a] = 0x20;
    q[0x148] = 0x20;
    q[0x149] = 0xf0;
    q[0x14a] = 0x20;
    *(s16 *)(q + 0x150) = 0x30;
    data_80171c5d = 6;
  }
  else if (right->field_d8 != 0 && (s16)c >= 0x60)
  {
    *(s16 *)(base + (j << 4) + 0x150) = 0x30;
  }
  c = right->field_c6;
  if ((unsigned)(c - 0x60) < 0x30)
  {
    *(s16 *)(base + (j << 4) + 0x170) = c - 0x60;
  }
  else if (data_80171c5d == 6)
  {
    u8 *q = (u8 *)((j << 4) + (int)base);
    q[0x128] = 0xf0;
    q[0x129] = 0xf0;
    q[0x12a] = 0x20;
    q[0x148] = 0xf0;
    q[0x149] = 0xf0;
    q[0x14a] = 0x20;
    *(s16 *)(q + 0x170) = 0x30;
    data_80171c5d = 7;
  }
  else if (right->field_d8 != 0 && (s16)c >= 0x90)
  {
    *(s16 *)(base + (j << 4) + 0x170) = 0x30;
    data_80171c5f = 1;
  }
  if (data_80171c5f != 0)
  {
    func_8013411c(base, j);
  }
  if (right->field_d8 != 0)
  {
    right->field_c6 = (s16)right->field_c6 / 3;
  }
  func_80133848(base, left, right);
}
