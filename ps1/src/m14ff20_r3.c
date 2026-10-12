/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150b80(Menu *menu, int a) {
    int lo = a & 0xff;
    int hi = (a & 0xff00) >> 8;
    menu->cd_param = 1;
    menu->track_code = a;
    menu->low_byte = lo;
    func_80150c6c(menu, data_8017eb34[hi]);
    menu->cd_sector = menu->end_sector = lo + menu->cd_sector;
    menu->end_sector = (data_8017eb44[hi * 8 + lo] << 3) + menu->end_sector;
    func_8015cec4(menu->cd_sector, &menu->cd_position);
    set_cd_volume(0);
    func_8016a730(0, 0, 1);
    func_8016904c(0, 0x55, 0x55);
    menu->mode_index = 2;
    menu->command_state = 7;
}
