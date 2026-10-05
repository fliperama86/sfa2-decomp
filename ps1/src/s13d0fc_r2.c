/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


u8 func_8013d21c(Object *object, u8 index, u8 arg) {
    table_8017abb8[object->slots[index].field_00](object, index, arg);
    return data_80188f44;
}
