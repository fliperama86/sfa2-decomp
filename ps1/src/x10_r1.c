/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_801511c8(void)
{
  int i;
  int j;
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 26; j++)
    {
      func_8015c09c(&data_8018947c[i].pairs[j].polys[0]);
      func_8015c09c(&data_8018947c[i].pairs[j].polys[1]);
      (&data_8018947c[i].pairs[j])->polys[0].field_04 = 0x80;
      (&data_8018947c[i].pairs[j])->polys[0].field_05 = 0x80;
      (&data_8018947c[i].pairs[j])->polys[0].field_06 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).field_04 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).field_05 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).field_06 = 0x80;
      data_8018947c[i].pairs[j].polys[0].field_16 = func_8015bd0c(0, 0, 0x3c0, 0);
      data_8018947c[i].pairs[j].polys[1].field_16 = func_8015bd0c(0, 0, 0x3c0, 0);
      data_8018947c[i].pairs[j].polys[0].field_0e = func_8015bdd4(0x60, 0x1e0);
      data_8018947c[i].pairs[j].polys[1].field_0e = func_8015bdd4(0x60, 0x1e0);
    }
  }
}
