/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015a560(void);

/* The parameter is unused here. It is declared because the one caller found so far, in the module of slot 0x27, sets the first argument register before the call; that is compatible with a parameter and does not prove one. */
void func_8011ea68(u8 *unused) {
    int buf[5];
    func_8015a560();
    func_8015a570(buf);
    func_80157d9c(0);
    func_80157fc4(buf[3], buf[4]);
    if (buf[0] & 8) {
        func_80157fc4(buf[1], buf[2]);
    }
    func_80157d9c(0);
}
