/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80150c6c(Node *sprite, s16 index) {
    sprite->field_30 = table_8017d9e4[index].b;
    sprite->field_34 = table_8017d9e4[index].c;
    sprite->field_28 = table_8017d9e4[index].a;
}
