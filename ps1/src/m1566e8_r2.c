/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80157090(void *a, Object *object) {
    int i;
    for (i = 0; i < 25; i++) {
        if (object->name_char_0 == table_801818d0[i * 3] &&
            object->name_char_1 == table_801818d0[i * 3 + 1] &&
            object->name_char_2 == table_801818d0[i * 3 + 2]) {
            object->name_char_0 = 0x43;
            object->name_char_1 = 0x41;
            object->name_char_2 = 0x50;
            object->name_char_3 = 0x20;
            return;
        }
    }
    if (object->name_char_0 == 0x4b) {
      if (object->name_char_1 == 0x41) {
        if (object->name_char_2 == 0x57) {
        object->name_char_0 = 0x54;
        object->name_char_1 = 0x4c;
        object->name_char_2 = 0x44;
        object->name_char_3 = 0x20;
        }
      }
    }
}
