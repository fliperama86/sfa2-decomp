/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011cbb8(Slab172 *p) {
    u16 *cur = (u16 *)(((p->sequence->field_04 >> 1) << 1) + p->field_9c);
    u16 *q;
    if (p->field_80 != 0 || data_80183d74[p->field_02][0] != p->field_0b) {
        p->field_80 = 0;
        if (data_80183dec[p->field_02][0] != (u32)cur || data_80183d74[p->field_02][0] != p->field_0b) {
            func_8011cdc0(p);
            if (*cur != 0) {
                func_8011cf98(p, cur);
            } else {
                func_8011d5b8(p, cur);
            }
            func_80119d88(p);
            q = func_8011bdc0(p);
            if (*q != 0 && q != cur) {
                func_8011cf98(p, q);
            } else {
                func_8011d5b8(p, q);
            }
        } else {
            func_80119d88(p);
            q = func_8011bdc0(p);
            if (*q != 0 && q != cur) {
                func_8011cf98(p, q);
            } else {
                func_8011d5b8(p, q);
            }
        }
    } else {
        func_80119ddc(p);
    }
    func_8011d74c(p, 0, 0, p);
    func_8011db78(p, 0, 0, p);
}
