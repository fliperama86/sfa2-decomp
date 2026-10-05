/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


Block172 *func_8011f1e0(void) {
    Block172 *b;
    Block172 *result;
    if (data_80197f10 != 0) {
        b = table_80197f20[data_80197f10];
        data_80197f10--;
        func_80119b34(b);
        result = b;
    } else {
        result = 0;
    }
    return result;
}
