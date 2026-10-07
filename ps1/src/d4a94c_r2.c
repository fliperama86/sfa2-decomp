/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"

extern InputLog input_log;
extern s8 data_801a6984, data_801a6985, data_801a6987;
extern u16 data_801a6966, data_801a6972;
extern u16 data_80185ff8, data_80185ffc;
extern u16 table_80185fd8[2][8];
extern u8 table_8016e664[2][8];
extern u16 data_80171b32, data_80171b34;
extern s16 data_80171b36;
extern s16 table_80171b38[4], table_80171b40[4];
extern Pair table_80171b48[];

u8 func_80148e84(Object *object);
u8 func_80148ea8(Object *object);
int func_80151184(void);
u8 func_80146840(Object *object);
u8 func_80146864(Object *object);
void func_80131ab4(Object *object);

void func_8013172c(Object *object)
{
  unsigned a;
  unsigned b;
  u16 c;
  u16 d;
  u16 x;
  s16 y;
  int new_var;
  u8 *g = &game_state.field_30;
  if ((*g) != 0)
  {
    new_var = 0x15;
    if (g[-new_var] != (object->side + 1))
    {
      *(u16 *)((u8 *)object + 0xc2) = 0;
      *(u16 *)((u8 *)object + 0x130) = 0;
      *(u16 *)((u8 *)object + 0x150) = 0;
      if (data_801a6987 != 0)
      {
        object->field_c2 = 0x4000;
        object->field_130 = 0x4000;
        object->field_150 = 0x4000;
      }
    }
    else
      if (data_801a6985 != 0)
    {
      func_80131ab4(object);
    }
  }
  x = object->field_130;
  y = (s16) object->field_150;
  a = (x >> 2) & 0x2000;
  b = (x & 0x2000) << 2;
  c = (y & 0x8000) >> 2;
  d = (y & 0x2000) << 2;
  if (object->field_0b != 0)
  {
    object->field_130 = (x & 0x5fff) + (a + b);
  }
  if (object->field_158 != 0)
  {
    object->field_150 = (object->field_150 & 0x5fff) + (c + d);
  }
  a = object->field_132;
  a = ~a;
  a &= object->field_130;
  object->field_134 = a;
  a = object->field_130;
  a = ~a;
  a &= object->field_132;
  object->field_136 = a;
}
