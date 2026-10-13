/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

/* Finished by hand from a candidate that the automatic permutation search had reshaped. */
void draw_text_items(void)
{
  u16 line;
  int i;
  data_8018d13c = (Cmd *)data_8018a4bc[draw_buffer_index];
  data_8018d140 = data_8018ccbc[draw_buffer_index];
  for (i = 0; i < data_8018d204; i++)
  {
    TextItem *item = table_8018d144[i];
    u16 y = item->y;
    int pal = item->palette;
    u16 x0 = item->x;
    u8 *base = (u8 *) (item->table_index << 2) + (int) current_ordering_table;
    u8 c = item->char_code;
    unsigned short step = item->step;
    u16 x = x0;
    u8 *s;
    line = item->line;
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
              add_glyph_8x8_prim((int)base, c, x, y);
              break;

            case 1:
              add_glyph_8x16_prim((int)base, c, x, y);
              break;

            case 2:
              add_glyph_16x16_prim((int)base, c, x, y);
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
              add_glyph_8x8_prim((int)base, c, x, y);
              break;

            case 1:
              add_glyph_8x16_prim((int)base, c, x, y);
              break;

            case 2:
              add_glyph_16x16_prim((int)base, c, x, y);
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
          add_glyph_16x16_prim_paged(base, c, *(s++), x, y);

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
