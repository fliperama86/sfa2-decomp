/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8016904c(unsigned char a, short b, short c) {
    PadReq req;
    if (a == 0) {
        req.field_00 = 0xc0;
        if (b >= 0x80) {
            b = 0x7f;
        }
        if (c >= 0x80) {
            c = 0x7f;
        }
        req.field_10 = (b * 32767) / 127;
        req.field_12 = (c * 32767) / 127;
    }
    if (a == 1) {
        req.field_00 = 0xc00;
        if (b >= 0x80) {
            b = 0x7f;
        }
        if (c >= 0x80) {
            c = 0x7f;
        }
        req.field_1c = (b * 32767) / 127;
        req.field_1e = (c * 32767) / 127;
    }
    func_8016b364(&req);
}
