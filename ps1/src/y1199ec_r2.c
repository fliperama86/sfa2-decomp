/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80119b3c(AnimObj *obj) {
    func_8011a6b8(data_80183ffc[obj->field_94][0], data_801841bc[obj->field_94][0]);
    func_8011a6b8(data_80183ffc[obj->field_94][1], data_801841bc[obj->field_94][1]);
    data_801841bc[data_80183ffc[obj->field_94][0]][0] = 0;
    data_801841bc[data_80183ffc[obj->field_94][1]][0] = 0;
    data_801841bc[data_80183ffc[obj->field_94][0]][1] = 0;
    data_801841bc[data_80183ffc[obj->field_94][1]][1] = 0;
    data_8018437c[data_80183ffc[obj->field_94][0]][0] = 0;
    data_8018437c[data_80183ffc[obj->field_94][1]][0] = 0;
}
