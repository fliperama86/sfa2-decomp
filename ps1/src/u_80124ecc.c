/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_80125268(void);

/* Form found by automatic permutation search. */
void func_80124ecc(void)
{
  int n;
  int v;
  int new_var;
  func_80125dc0(0, 0x20, 0xd, 0);
  func_80125dc0(1, 0x20, 0xd, 0);
  func_80125dc0(2, 0x20, 0xd, 0);
  func_80125dc0(3, 0x20, 0xd, 0);
  func_80137b10();
  game_state.field_225 = 0;
  v = 0x700;
  func_8014f4d4(1, v);
  while ((data_80190949 == 0) || (game_state.field_f0 != 0))
  {
    func_8011a784();
    func_801192bc(1);
  }

  n = 0x77;
  do
  {
    v = 0x1a;
    if (func_80125268() != 0)
    {
      func_8014f4d4(6, 2);
      break;
    }
    if (n & 2)
    {
      data_8016e863 = v;
    }
    else
    {
      data_8016e863 = 0x10;
    }
    func_801519b4(&data_8016e858);
    func_8011a784();
    func_801192bc(1);
  }
  while (new_var = n--);
  func_801260ac(0, 0x20, 0);
  func_801260ac(1, 0x20, 0);
  ;
  func_801260ac(2, 0x20, 0);
  func_801260ac(3, 0x20, 0);
  func_80137b10();
}
