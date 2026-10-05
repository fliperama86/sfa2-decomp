/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80165d34(short a, short b) {
    int buf[10];
    buf[0] = 3;
    ((short *)buf)[2] = a * 129;
    ((short *)buf)[3] = b * 129;
    func_8016b364(buf);
}
