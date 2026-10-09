/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80124a7c(u8 index) {
    data_801a6986 = 0;
    data_801a6987 = 0;
    data_801a6988 = 0;
    if (index == 0) {
        data_801a6986 = 1;
    } else if (index == 1) {
        data_801a6987 = 1;
    } else if (index == 2) {
        data_801a6988 = 1;
    }
}

void func_80124ae8(u8 index) {
    game_state.field_56 = table_8016e81c[index];
}

void func_80124b10(void) {
    data_80197f1c = 0;
    ((HudBig *)data_8018f5a0)->field_52 = 0;
    game_state.field_65 = 0;
    func_8011eae4();
    func_80136c8c();
    func_8013245c();
    func_801285e0();
    func_801260ac(0, 0x20, 0);
    func_801260ac(1, 0x20, 0);
    func_801260ac(2, 0x20, 0);
    func_801260ac(3, 0x20, 0);
    func_801260ac(4, 0x20, 0);
    func_80137b10();
}
