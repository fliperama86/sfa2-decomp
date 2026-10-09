/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015a560(u32 *p);

/* The parameter is passed on to func_8015a560, which stores it: the original sets no argument register before that call, so the callee receives what this function's caller passed. The one caller found so far, in the module of slot 0x27, passes a pointer; that is compatible with this parameter and does not prove its type. */
void func_8011ea68(u8 *p) {
    int buf[5];
    func_8015a560((u32 *)p);
    func_8015a570(buf);
    func_80157d9c(0);
    func_80157fc4(buf[3], buf[4]);
    if (buf[0] & 8) {
        func_80157fc4(buf[1], buf[2]);
    }
    func_80157d9c(0);
}
