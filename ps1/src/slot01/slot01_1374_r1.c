/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u16 data_801a2be4[];
int func_8001336c_slot01(u8 a);

void func_80011374_slot01(void) {
    Rect r;
    r.x = 0;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4);
    func_80157d9c(0);
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0x400);
    func_80157d9c(0);
    r.x = 0x20;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0x800);
    func_80157d9c(0);
    r.x = 0x60;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 - 0x400);
    func_80157d9c(0);
    r.x = 0x70;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, (u8 *)data_801a2be4 + 0xc00);
    func_80157d9c(0);
    if (func_8001336c_slot01(3) != 0) {
        data_8018f5a0->field_4e++;
        game_state.field_06 = 0xff;
    }
}
