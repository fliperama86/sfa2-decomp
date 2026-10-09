/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


/* Adds a pointer to the queue table_8018d144 and does not look at what
   it points to. Callers pass records that they hold under several struct
   names; the one function of this tree that reads the queue,
   func_80151a04, reads each entry as a TextItem. */
void func_801519b4(void *item) {
    if (game_state.field_74 == 0) {
        if (data_8018d204 < 0x30) {
            table_8018d144[data_8018d204++] = item;
        }
    }
}
