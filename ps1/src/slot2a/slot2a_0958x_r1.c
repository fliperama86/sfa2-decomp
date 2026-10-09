/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801e0958_slot2a(Object *obj, SpritePrim *prim) {
    SpriteSet *set = (SpriteSet *)obj->sequence->field_04;
    int count = set->count;
    u16 *offs = set->offsets;
    u8 *codes = set->codes;
    u32 *ot;
    u8 q, r;
    short s4, s3; int idx1, idx2, idx3, x1, x2, x3, x4, y1, y2, y3;
    short a; short b;
    int dx, dy;
    u32 c;

    do {
        c = *codes++;
        q = c / 6;
        r = c % 6;
        s4 = (r << 4) + obj->field_03 * 96;
        s3 = q << 4;
        if (player_left.field_cd == 0) {
            if (obj->field_0b == 0) {
                idx1 = obj->field_3c->field_d4 * 3 + *codes++;
                y1 = idx1 / 4;
                x1 = (idx1 - y1 * 4) * 16 + 0x2c0;
                y1 += 0x60;
                prim->clut = (u16)func_8015bdd4(x1, y1);
            } else {
                idx2 = obj->field_3c->field_d4 * 3 + *codes++ + 2;
                y2 = idx2 / 4;
                x2 = (idx2 - y2 * 4) * 16 + 0x2c0;
                y2 += 0x64;
                prim->clut = (u16)func_8015bdd4(x2, y2);
            }
        } else {
            if (obj->field_0b == 0) {
                idx3 = obj->field_3c->field_d4 * 3 + *codes++ + 2;
                y3 = idx3 / 4;
                x3 = (idx3 - y3 * 4) * 16 + 0x2c0;
                y3 += 0x64;
                prim->clut = (u16)func_8015bdd4(x3, y3);
            } else {
                idx1 = obj->field_3c->field_d4 * 3 + *codes++;
                prim->clut = (u16)func_8015bdd4((idx1 % 4) * 16 + 0x2c0, idx1 / 4 + 0x60);
            }
        }
        a = *codes++;
        a <<= 4;
        b = *codes++;
        b <<= 4;
        dx = *offs++;
        dy = *offs++;
        if (obj->field_0b != 0) {
            prim->x0 = (u16)obj->pos_x - dx;
            prim->y0 = dy + (u16)obj->pos_y;
            prim->x1 = (u16)obj->pos_x - dx - a;
            prim->y1 = dy + (u16)obj->pos_y;
            prim->x2 = (u16)obj->pos_x - dx;
            prim->y2 = dy + (u16)obj->pos_y + b;
            prim->x3 = (u16)obj->pos_x - dx - a;
            prim->y3 = dy + (u16)obj->pos_y + b;
            prim->u0 = s4;
            prim->v0 = s3;
            prim->u1 = s4 + a - 1;
            prim->v1 = s3;
            prim->u2 = s4;
            prim->v2 = s3 + b;
            prim->u3 = s4 + a - 1;
            prim->v3 = s3 + b;
        } else {
            prim->x0 = dx + (u16)obj->pos_x;
            prim->y0 = dy + (u16)obj->pos_y;
            prim->x1 = dx + (u16)obj->pos_x + a;
            prim->y1 = dy + (u16)obj->pos_y;
            prim->x2 = dx + (u16)obj->pos_x;
            prim->y2 = dy + (u16)obj->pos_y + b;
            prim->x3 = dx + (u16)obj->pos_x + a;
            prim->y3 = dy + (u16)obj->pos_y + b;
            prim->u0 = s4;
            prim->v0 = s3;
            prim->u1 = s4 + a;
            prim->v1 = s3;
            prim->u2 = s4;
            prim->v2 = s3 + b;
            prim->u3 = s4 + a;
            prim->v3 = s3 + b;
        }
        ot = (u32 *)data_801987c8;
        ((PrimTag *)prim)->addr = ((PrimTag *)&ot[obj->field_03])->addr;
        ((PrimTag *)&ot[obj->field_03])->addr = (u32)prim;
        prim++;
    } while (--count != 0);
}
