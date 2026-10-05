/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8013786c(Object *object) {
    data_8018daf4[object->side] = 1;
}

void func_8013788c(Object *object) {
    if (object->side == 0) {
        func_80136e90();
    } else {
        func_80136ed0();
    }
    func_80137220(0, 6);
}
