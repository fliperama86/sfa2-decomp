/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011bc84(Slab172 *p) {
    u16 *cur = (u16 *)(((p->sequence->data_ptr >> 1) << 1) + p->stream_base);
    u16 *old;
    u16 *q;
    if (p->reload_flag != 0) {
        p->reload_flag = 0;
        old = data_8018437c[p->stream_index][0];
        if (old != cur) {
            func_8011be18(p);
            if (*cur != 0) {
                func_8011bf70(p, cur);
            } else {
                func_8011c568(p, cur);
            }
            func_80119d88(p);
            q = func_8011bdc0(p);
            if (*q != 0 && q != cur) {
                func_8011bf70(p, q);
            } else {
                func_8011c568(p, q);
            }
        } else {
            func_80119d88(p);
            q = func_8011bdc0(p);
            if (*q != 0 && q != old) {
                func_8011bf70(p, q);
            } else {
                func_8011c568(p, q);
            }
        }
    } else {
        func_80119ddc(p);
    }
    func_8011c724(p);
}
