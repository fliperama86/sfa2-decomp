/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80017540_slot12[];
extern u8 data_80028aa0_slot12[];
extern u16 *data_80028b40_slot12;
extern u16 *data_80028b44_slot12;

void func_800113e4_slot12(void) {
    Rect r;
    u8 tbl[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 17, 2};
    Object *obj = game_state.field_154;
    u8 *buf;
    int x;

    r.x = 0;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0xf;
    func_80157fc4(&r, (u8 *)0x80042284);
    func_80157d9c(0);
    r.x = 0;
    r.y = 0x1f1;
    r.w = 0x10;
    r.h = 1;
    func_80157fc4(&r, data_80017540_slot12);
    func_80157d9c(0);
    x = 0x100;
    if (obj->side != 0) {
        x = 0x120;
    }
    r.x = x;
    r.y = 0x1e0 + obj->field_d4 * 5;
    r.w = 0x10;
    r.h = 5;
    func_80158028(&r, data_80028aa0_slot12);
    func_80157d9c(0);
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 1;
    if (obj->kind == 0x14) {
        func_80157fc4(&r, (u8 *)0x80043284 + obj->field_d4 * 32);
    } else {
        func_80157fc4(&r, (u8 *)0x80042444 + tbl[obj->kind] * 32 + obj->field_d4 * 19 * 32);
    }
    func_80157d9c(0);
    buf = data_801a2fe4;
    data_80028b40_slot12 = (u16 *)buf;
    data_80028b44_slot12 = (u16 *)(buf + 0x1400);
    r.y = 0x1f0;
    r.x = 0;
    r.w = 0x10;
    r.h = 1;
    func_80157fc4(&r, data_80017540_slot12);
    func_80157d9c(0);
    r.x = 0;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, buf - 0x400);
    func_80157d9c(0);
    func_80158028(&r, buf + 0x1000);
    func_80157d9c(0);
    r.x = 0x10;
    r.y = 0x1e0;
    r.w = 0x10;
    r.h = 0x20;
    func_80158028(&r, buf);
    func_80157d9c(0);
    func_80158028(&r, buf + 0x1400);
    func_80157d9c(0);
}
