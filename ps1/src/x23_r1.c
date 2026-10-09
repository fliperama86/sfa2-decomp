/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void func_80151a04(void)
{
  u16 line;
  int i;
  data_8018d13c = (Cmd *)data_8018a4bc[data_801a27d0];
  data_8018d140 = data_8018ccbc[data_801a27d0];
  for (i = 0; i < data_8018d204; i++)
  {
    TextItem *item = table_8018d144[i];
    u16 y = item->field_06;
    int pal = item->field_0b;
    u16 x0 = item->field_04;
    u8 *base = (u8 *) (item->field_00 << 2) + (int) data_801987c8;
    u8 c = item->field_0a;
    unsigned short step = item->field_08;
    u16 x = x0;
    u8 *s;
    line = item->field_09;
    if ((c < 2) || (c == 2))
    {
      u8 mode = c;
      data_8018d208 = func_8015bdd4(0x70, pal + 0x1e0);
      data_8018d20c = func_8015bd0c(0, 0, 0x3c0, 0x100);
      s = item->text;
      do
      {
        c = *(s++);
        switch (c)
        {
          case 0x7c:
            c = *(s++);
            switch (mode)
          {
            case 0:
              func_80151f64((int)base, c, x, y);
              break;

            case 1:
              func_80151fec((int)base, c, x, y);
              break;

            case 2:
              func_80152088((int)base, c, x, y);
              break;

          }

            c = 0;
            x = x + step;
            continue;

          case 0x2f:
            x = x0;
            y = y + line;
            continue;

          case 0x40:
            c = *(s++);
            switch (c)
          {
            case 0x63:
              pal = *(s++);
              data_8018d208 = func_8015bdd4(0x70, pal + 0x1e0);
              break;

            case 0x43:
              data_8018d208 = func_8015bdd4(*s++ << 4, pal + 0x1e0);
              break;

            case 0x40:
              continue;

            default:
              goto other;

          }

            continue;

          case 0x20:
            x = x + step;
            continue;

          default:
            other:
          switch (mode)
          {
            case 0:
              func_80151f64((int)base, c, x, y);
              break;

            case 1:
              func_80151fec((int)base, c, x, y);
              break;

            case 2:
              func_80152088((int)base, c, x, y);
              break;

          }


            x = x + step;
            continue;

        }

        x = x + step;
      }
      while (c != 0x40);
      func_80158a2c(data_8018d140, 1, 1, data_8018d20c, 0);
      func_8015bf34((int) base, (Cmd *) data_8018d140++);
    }
    else
    {
      u8 b;
      data_8018d208 = func_8015bdd4(((u8 *) item)[0xd] << 4, pal + 0x1e0);
      b = ((u8 *) item)[0xc];
      data_8018d20c = func_8015bd0c(0, 0, (b & 0xf) << 6, (b & 0x10) << 4);
      s = item->text2;
      do
      {
        c = *(s++);
        if (c == 0xff)
        {
          c = *(s++);
          switch (c)
          {
            case 0xfe:
              x = x0;
              y = y + line;
              break;

            case 0xfd:
              data_8018d208 = func_8015bdd4(*(s++), pal + 0x1e0);
              break;

            case 0xfc:
              func_80158a2c(data_8018d140, 1, 1, data_8018d20c, 0);
              func_8015bf34((int) base, (Cmd *) data_8018d140++);
              b = *(s++);
              data_8018d20c = func_8015bd0c(0, 0, (b & 0xf) << 6, (b & 0x10) << 4);
              break;

            case 0xfb:
              data_8018d208 = func_8015bdd4(((u8 *) item)[0xd] << 4, (*(s++)) + 0x1e0);
              break;

            case 0xff:
              break;

            default:
              goto draw;

          }

        }
        else
        {
          draw:
          func_80152124(base, c, *(s++), x, y);

          x = x + step;
        }
      }
      while (c != 0xff);
      func_80158a2c(data_8018d140, 1, 1, data_8018d20c, 0);
      func_8015bf34((int) base, (Cmd *) data_8018d140++);
    }
  }

  data_8018d204 = 0;
}
