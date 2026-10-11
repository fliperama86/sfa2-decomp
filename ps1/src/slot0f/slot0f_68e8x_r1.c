/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 *data_800efdc8_slot0f[];
extern u16 *data_800efef8_slot0f[];
extern u16 data_800eff58_slot0f[];

/* What it does (inferred): fills in two adjacent sprite records for an
   object. idx (its low 16 bits) selects a list of halfwords: entry idx of
   data_800efdc8_slot0f, or for idx 2 entry obj->field_48 of
   data_800efef8_slot0f. The list gives the texture coordinates and two
   values for a library call whose result goes into the command word; the
   position is obj->pos_x plus data_800eff58_slot0f[idx] and obj->pos_y, the
   page 0x7f07. */
void func_800e68e8_slot0f(Object *obj, Slot12Prim *prim, int idx) {
    Slot12Prim *next = prim + 1;
    u16 *list;
    u16 u;
    u16 v;
    int r;

    if ((u16)idx != 2) {
        list = data_800efdc8_slot0f[(u16)idx];
    } else {
        list = data_800efef8_slot0f[obj->field_48];
    }
    /* The list is walked: written v = list[1] with list += 6 below, this function differs from the original in 7 instruction lines. */
    u = list[0];
    list++;
    v = list[0];
    prim->u = u;
    prim->v = v;
    next->u = u;
    next->v = v;
    prim->x = obj->pos_x + data_800eff58_slot0f[(u16)idx];
    prim->y = obj->pos_y;
    list += 5;
    next->x = obj->pos_x + data_800eff58_slot0f[(u16)idx];
    next->y = obj->pos_y;
    prim->tpage = 0x7f07;
    next->tpage = 0x7f07;
    /* u holds the first of the call's two values here and the list is stepped before the second is read: with a local of its own for the value this function differs from the original in 8 instruction lines, and written without the step, with list[1] in the call, in 26. */
    u = list[0];
    list++;
    r = func_8015bd0c(0, 0, (s16)u, (s16)list[0]);
    prim->cmd = 0xe1000000 + (((Object *)data_801987c8)->field_a2 << 9) + (((Object *)data_801987c8)->field_a3 << 10) + (r & 0x1f);
    next->cmd = 0xe1000000 + (((Object *)data_801987c8)->field_a2 << 9) + (((Object *)data_801987c8)->field_a3 << 10) + (r & 0x1f);
}
