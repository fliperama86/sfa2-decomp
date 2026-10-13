/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u8 data_8019d7cc;
extern u32 data_801e7668_slot0b;

/*
 * What it does (inferred, not an original name): draws a tile map as sprite
 * records. A header reached through the object's sequence describes a grid of
 * 16-bit cells (columns, rows, a texture page offset, the x and y origin,
 * then the cells). For each cell whose low 14 bits are not 0 it initialises a
 * 0x1c-byte record through init_draw_mode_prim_3, fills in the position (the object's
 * pos_x less box_margin, pos_y plus view_scroll_y less 8, 16 pixels per cell,
 * less the origin), the texture coordinates and command word from the cell,
 * grey colour 0x80 and the texture page, and links the record into the list
 * head selected by the object. Records are taken in order from one of two
 * buffers chosen by draw_buffer_index.
 */
void draw_tilemap_cells_from_sequence_slot0b(Object *obj) {
    Tilemap *map = (Tilemap *)obj->sequence->data_ptr;
    u32 *ot;
    u32 *p = (u32 *)&data_8019d7cc;
    s16 x0;
    u16 w;
    u16 h;
    s16 tpage;
    int y;
    int row;
    int col;
    int cell;

    tpage = ((obj->image_row + map->page_offset) << 6) + 0x7808;
    x0 = obj->pos_x - box_margin[0];
    /* The listing loads pos_y unsigned; the plain obj->pos_y differs in 1 instruction line. */
    y = (u16)obj->pos_y + view_scroll_y[0] - 8;
    ot = (u32 *)current_ordering_table + obj->ot_depth;
    w = map->field_04;
    h = map->field_06;
    data_801e7668_slot0b = 0;
    if ((u8)draw_buffer_index != 0) {
        p += 0x930;
    }
    for (row = 0; row < map->rows; row++) {
        int ry = y + row * 16;
        for (col = 0; col < map->cols; col++) {
            TilePrim *q = (TilePrim *)(p + 1);
            /* ty and rem as 16 bits; as int they differ in 30 and 12 instruction lines. */
            s16 ty = ry - h;
            s16 rem;
            u8 u;
            u8 v;
            /* Plain map->cells[col + row * map->cols] differs in 1 instruction line. */
            cell = *(u16 *)((u8 *)map + ((col + row * map->cols) << 1) + 8) & 0x3fff;
            if (cell == 0)
                continue;
            /* The original has two more instruction slots here, at the division by 256 and by 16, that
               correct a negative value; a cell is masked to 14 bits and is never negative, so no input
               reaches them. Computing v before u: the other order differs in 44 instruction lines. */
            rem = cell % 256;
            v = rem % 16;
            u = rem / 16;
            init_draw_mode_prim_3((Tx *)p);
            q->tpage = tpage;
            q->x = x0 + col * 16 - w;
            q->y = ty;
            q->u = u * 16;
            q->v = v * 16;
            q->color_r = 0x80;
            q->color_g = 0x80;
            q->color_b = 0x80;
            q->cmd = 0xe1000006 + cell / 256;
            ((PrimTag *)p)->addr = ((PrimTag *)ot)->addr;
            ((PrimTag *)ot)->addr = (u32)p;
            p += 7;
        }
    }
}
