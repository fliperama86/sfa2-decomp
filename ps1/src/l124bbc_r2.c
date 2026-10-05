/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801257f4(Select *sel) {
    Object *first = &player_left;
    Object *second = first + 1;
    Object *tmp;
    u8 flag;
    s16 i;
    u8 j;
    u32 mask;
    int flags;

    if (sel->field_1b == 3) {
        if (sel->field_07 == 3) {
            if (sel->field_71 == 0) {
                tmp = first;
                first = second;
                second = tmp;
            }
        } else if (sel->field_07 & 1) {
            tmp = first;
            first = second;
            second = tmp;
        }
    } else {
        if (!(sel->field_1b & 1)) {
            tmp = first;
            first = second;
            second = tmp;
        }
        func_80125938(sel, first, second);
        i = -1;
        flags = sel->field_50;
    again:
        i++;
        flag = sel->field_120[i];
        if (!(i < sel->field_a4)) {
            goto out;
        }
        j = 0;
        mask = 1;
        for (; j < flag; j++) {
            mask <<= 1;
        }
        if (mask & flags) {
            goto again;
        }
    out:
        sel->field_54 = i;
        second->kind = flag;
    }
    sel->field_40 = (u8)func_80125afc(second->kind);
    func_80125b34(sel, first);
}
