/* Reconstruction. Names/roles inferred, not original symbols. */
#ifndef GAME_H
#define GAME_H

#include "object.h"

/* The first word of a drawing primitive and of an ordering-table entry:
   the address of the next entry in the low 24 bits, a length in words in
   the high 8. Written with bit-fields because that is the form whose loads
   and stores this compiler emits in the order the original has. The layout
   is inferred from the code; the declaration is not an original one. */
typedef struct {
    unsigned addr : 24;
    unsigned len : 8;
} PrimTag;

extern GameState game_state;

#endif
