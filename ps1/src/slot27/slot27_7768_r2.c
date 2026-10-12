/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_draw_mode_prim_with_merge_slot27(Tx *tx) {
    func_80158a2c((Prim *)tx, 0, 0, 0, 0);
    tx->mode_id = 3;
    tx->prim_code = 0x74;
    func_8015c23c(tx, &tx->prim_data);
}
