/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80124bbc(Object *object, int b, u16 c) {
    if (*table_8016e820[object->side] & 0x20) {
        data_80185fb8 = 1 << object->side;
    }
    if (data_80185fb8 == 0) {
        if (*table_8016e820[object->side] & 0x1000) {
            int old;
            func_80120554(0, 0, 0x34d);
            old = data_80185fb4;
            if (old == 0) {
                data_80185fb4 = c;
            } else {
                data_80185fb4 = old - 1;
                if (data_801ae028 == 0 && data_80185fb4 == 2) {
                    data_80185fb4 = old - 2;
                }
            }
        }
        if (*table_8016e820[object->side] & 0x4000) {
            u16 old;
            func_80120554(0, 0, 0x34d);
            old = data_80185fb4;
            if (old == c) {
                data_80185fb4 = 0;
            } else {
                data_80185fb4 = old + 1;
                if (data_801ae028 == 0 && data_80185fb4 == 2) {
                    data_80185fb4 = old + 2;
                }
            }
        }
    }
}

void func_80124d70(Object *object, int b, u16 c) {
    if (*table_8016e828[object->side] & 0xf0) {
        data_80185fb8 = 1 << object->side;
    }
    if (data_80185fb8 == 0) {
        if (*table_8016e828[object->side] & 0x8000) {
            int old;
            func_80120554(0, 0, 0x34d);
            old = data_80185fb4;
            if (old == 0) {
                data_80185fb4 = c;
            } else {
                data_80185fb4 = old - 1;
            }
        }
        if (*table_8016e828[object->side] & 0x2000) {
            u16 old;
            func_80120554(0, 0, 0x34d);
            old = data_80185fb4;
            if (old == c) {
                data_80185fb4 = 0;
            } else {
                data_80185fb4 = old + 1;
            }
        }
    }
}
