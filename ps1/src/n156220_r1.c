/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80156220(void) {
    u32 *p = &data_8018d270;
    *p = game_state.field_104;
    if (data_8018d26a != 0) {
        func_80156398(&data_80181668, data_8018d26c << 8);
        func_80156398(&data_80181684, data_8018d269);
        func_80156398(&data_801816a0, data_8018d274 << 8);
        func_80156398(&data_801816bc, *p << 8);
        func_801519b4(&data_801816d4);
        func_801519b4(&data_80181708);
        func_801519b4(&data_8018171c);
        func_801519b4(&data_80181758);
    } else {
        func_80156398(&data_80181684, data_8018d26c << 8);
        func_80156398(&data_801816a0, data_8018d269);
        func_80156398(&data_801816bc, *p << 8);
        func_801519b4(&data_801816e4);
        func_801519b4(&data_8018172c);
        func_801519b4(&data_8018173c);
    }
    func_801519b4(&data_80181774);
    func_801519b4(&data_8018178c);
}

void func_80156390(void) {
}
