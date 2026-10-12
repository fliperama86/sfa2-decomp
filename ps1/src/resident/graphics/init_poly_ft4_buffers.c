/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void init_poly_ft4_buffers(void)
{
  int i;
  int j;
  for (i = 0; i < 2; i++)
  {
    for (j = 0; j < 26; j++)
    {
      func_8015c09c(&data_8018947c[i].pairs[j].polys[0]);
      func_8015c09c(&data_8018947c[i].pairs[j].polys[1]);
      (&data_8018947c[i].pairs[j])->polys[0].r0 = 0x80;
      (&data_8018947c[i].pairs[j])->polys[0].g0 = 0x80;
      (&data_8018947c[i].pairs[j])->polys[0].b0 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).r0 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).g0 = 0x80;
      (*(&(&data_8018947c[i].pairs[j])->polys[1])).b0 = 0x80;
      data_8018947c[i].pairs[j].polys[0].tpage = func_8015bd0c(0, 0, 0x3c0, 0);
      data_8018947c[i].pairs[j].polys[1].tpage = func_8015bd0c(0, 0, 0x3c0, 0);
      data_8018947c[i].pairs[j].polys[0].clut = func_8015bdd4(0x60, 0x1e0);
      data_8018947c[i].pairs[j].polys[1].clut = func_8015bdd4(0x60, 0x1e0);
    }
  }
}
