/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80157090(void *a, Object *object) {
    int i;
    for (i = 0; i < 25; i++) {
        if (object->field_11c == table_801818d0[i * 3] &&
            object->field_11d == table_801818d0[i * 3 + 1] &&
            object->field_11e == table_801818d0[i * 3 + 2]) {
            object->field_11c = 0x43;
            object->field_11d = 0x41;
            object->field_11e = 0x50;
            object->field_11f = 0x20;
            return;
        }
    }
    if (object->field_11c == 0x4b) {
      if (object->field_11d == 0x41) {
        if (object->field_11e == 0x57) {
        object->field_11c = 0x54;
        object->field_11d = 0x4c;
        object->field_11e = 0x44;
        object->field_11f = 0x20;
        }
      }
    }
}
