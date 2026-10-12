/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

extern Quad data_80185c04[][8];
extern u8 data_80185504[][16];
void func_80158150(Quad *list, int n);

/*
 * What it does (inferred, not an original name): builds the draw commands of
 * a strip of 16-byte cells, once per change. A size value and a stream header
 * come from tables by the object's field_02 and a distance; a three-table
 * cache (side, column, buffer selector) skips the work when nothing changed.
 * Each header count fills a cell and registers it; every 16th cell also gets
 * a row primitive, and the size value moves on along a chain table.
 */
void func_8011d74c(Object *obj, u16 column, u16 frame, Block172 *second) {
    u16 idx = obj->field_02;
    u16 *call_block = (u16 *)func_801250c0(obj, frame);
    u16 dist = data_801900f8[idx] - *call_block + 1;
    u16 size;
    u16 *hdr;
    int count;
    u16 sel;
    Cell16 *cell;
    Quad *list;
    u16 col;
    Prim *prim;
    u32 i;

    if (dist < 10) {
        size = data_80183d9c[idx][dist];
        if (size == 0) {
            return;
        }
        hdr = (u16 *)data_80183dec[idx][dist];
        if (hdr == 0) {
            return;
        }
        count = *hdr;
        if (count == 0) {
            return;
        }
        if (data_80183dc4[idx][dist] == 0) {
            if (data_80183d9c[idx][dist] != data_80183d9c[idx][dist - 1]) {
                return;
            }
            if ((u32)hdr != data_80183dec[idx][dist - 1]) {
                return;
            }
        }
        sel = data_801a27d0;
        if (data_80185c84[obj->side][column][sel] == (int)hdr && data_80185cc4[obj->side][column][sel] == size && data_80185ce4[obj->side][column][sel] == second->field_0d) {
            return;
        }
        data_80185c84[obj->side][column][sel] = (int)hdr;
        data_80185cc4[obj->side][column][sel] = size;
        data_80185ce4[obj->side][column][sel] = second->field_0d;
        list = &data_80185c04[sel][obj->side * 4 + column];
        cell = &data_801987cc[sel][0x280 + obj->side * 320 + column * 80];
        prim = &data_8019056c[sel][(obj->side * 320 + column * 80) / 16];
        func_80158150(list, 2);
        col = 0;
        for (i = 0; i < count; i++) {
            cell->field_0c = size << 4;
            cell->field_0d = col << 4;
            cell->clut = func_8015bdd4(second->field_7a, second->field_7c + second->field_0d + data_80185504[size][col]);
            func_8015bf34((int)list, (Cmd *)cell++);
            col = (col + 1) & 0xf;
            if (col == 0) {
                func_80158a2c(prim, 1, 1, func_8015bd0c(0, 0, ((size >> 4) << 6) + 0x180, 0), 0);
                func_8015bf34((int)list, (Cmd *)prim++);
                size = data_80183c90[size];
            }
        }

        if (col != 0) {
            func_80158a2c(prim, 1, 1, func_8015bd0c(0, 0, ((size >> 4) << 6) + 0x180, 0), 0);
            func_8015bf34((int)list, (Cmd *)prim);
        }
    }
}
