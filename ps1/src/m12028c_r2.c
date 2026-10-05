/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80120a30(Object *o, int a, int b) {
    int c;
    if (o->field_293) {
        func_80120554(o, o->side, 0x32a);
    }
    if (a == 0) {
        if ((0x10d40 >> o->kind) & 1) {
            if (b == 0) {
                c = 0x318;
            } else {
                c = 0x34b;
            }
        } else {
            if (b == 0) {
                c = 0x317;
            } else {
                c = 0x34a;
            }
        }
    } else {
        if ((0x10d40 >> o->kind) & 1) {
            if (b == 0) {
                c = 0x317;
            } else {
                c = 0x34a;
            }
        } else {
            if (b == 0) {
                c = 0x316;
            } else {
                c = 0x349;
            }
        }
    }
    func_80120554(o, o->side, c);
}
