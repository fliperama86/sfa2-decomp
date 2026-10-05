#include "game.h"
#include "externs.h"
#include "protos.h"


/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_8014edac(int a)
{
  GameState *g = &game_state;
  g->field_225 = 1;
  func_80119144(2, 7);
  func_801192bc(1);
  func_801203b4(0);
  func_801203b4(1);
  func_801203b4(2);
  if (a == 0)
  {
    func_8014f3b8(4, 2);
  }
  else
  {
    func_8014f3b8(4, 1);
  }
  g->field_225 = 0;
  while (g->field_f0)
  {
    func_801192bc(1);
  }
}
