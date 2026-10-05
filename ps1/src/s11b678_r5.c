/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015a560(void);

void func_8011ea68(void) {
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
