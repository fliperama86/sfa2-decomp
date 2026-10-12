/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80150c6c(Node *sprite, s16 index) {
    sprite->table_b = table_8017d9e4[index].b;
    sprite->table_c = table_8017d9e4[index].c;
    sprite->table_a = table_8017d9e4[index].a;
}
