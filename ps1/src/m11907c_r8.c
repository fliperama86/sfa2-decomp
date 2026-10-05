/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f240(Slab172 *s) {
    if (s->field_02 == 10) {
        if (s->field_48 != 4) {
            data_801a27cc++;
        }
    }
    if (s->field_02 == 4) {
        data_801a27d4++;
    }
    func_80119b3c(s);
    func_8011f404(s, 0xac);
    s->field_44 = 0;
    s->field_08 = 12;
    data_80197f10++;
    s->field_26 = 0;
    s->field_28 = 0;
    s->field_2c = 0;
    s->field_30 = 0;
    s->field_34 = 0;
    table_80197f20[data_80197f10] = s;
}
