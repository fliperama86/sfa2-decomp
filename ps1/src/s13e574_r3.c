/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8013ead4(Object *object, int index, u8 arg) {
    table_8017ac08[object->slots[(u8)index].field_00](object, (u8)index, arg);
    return data_80188f44;
}
