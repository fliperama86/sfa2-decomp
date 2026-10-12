/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_table_function_by_state_0_5(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cd28[object->state_0](object);
}
