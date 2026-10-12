/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Cell16 data_800e929c_slot0f[];
extern int data_800f0200_slot0f;
extern int data_800f0204_slot0f;
extern int data_800f0208_slot0f;
extern int data_800f8578_slot0f;
void load_options_from_saved_data_slot0f(void);
void noop_empty_function_slot0f(void);
void store_and_darken_palette_row_slot0f(void);

void init_option_cells_and_load_saved_options_slot0f(void) {
    HudState *h = data_8018f5a0;
    int i;
    data_800f0200_slot0f = 0;
    data_800f0204_slot0f = 0;
    data_800f0208_slot0f = 1;
    data_800f8578_slot0f = 0;
    h->step_index = h->step_index + 1;
    load_options_from_saved_data_slot0f();
    noop_empty_function_slot0f();
    store_and_darken_palette_row_slot0f();
    for (i = 10; i >= 0; i--) {
        data_800e929c_slot0f[i].cell_value = 0x1a;
    }
    data_800e929c_slot0f[0].cell_value = 0x10;
}
