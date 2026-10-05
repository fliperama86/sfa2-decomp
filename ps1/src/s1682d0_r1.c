/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801682d0(short a, short b) {
    data_8018f6e4.field_00 = 6;
    data_8018f6e4.field_08 = (a * 32767) / 127;
    data_8018f6e4.field_0a = (b * 32767) / 127;
    func_8016a878(&data_8018f6e4);
}

void func_80168368(short a) {
    data_8018f6e4.field_00 = 16;
    data_8018f6e4.field_10 = a;
    func_8016a878(&data_8018f6e4);
}

void func_801683a8(short a) {
    data_8018f6e4.field_00 = 8;
    data_8018f6e4.field_0c = a;
    func_8016a878(&data_8018f6e4);
}
