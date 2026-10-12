/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f240(Slab172 *s) {
    if (s->field_02 == 10) {
        if (s->mode != 4) {
            data_801a27cc++;
        }
    }
    if (s->field_02 == 4) {
        data_801a27d4++;
    }
    func_80119b3c(s);
    clear_bytes(s, 0xac);
    s->field_44 = 0;
    s->type_code = 12;
    data_80197f10++;
    s->field_26 = 0;
    s->linked_entry_0 = 0;
    s->linked_entry_1 = 0;
    s->linked_entry_2 = 0;
    s->linked_entry_3 = 0;
    table_80197f20[data_80197f10] = s;
}
