/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80150cd0(int index) {
    Node *sprite = func_8014f194(table_8017ec9c[index]);
    sprite->field_20 = 0;
    func_801575cc(data_801900fc);
    func_801575dc();
    func_801578ac();
    func_8015782c(sprite, 1, 0);
    func_8015786c();
    func_801656ec();
    func_80165790();
}
