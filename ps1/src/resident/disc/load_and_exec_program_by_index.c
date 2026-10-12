/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void load_and_exec_program_by_index(int index) {
    Node *sprite = read_cd_file_by_index_into_memory(table_8017ec9c[index]);
    sprite->field_20 = 0;
    func_801575cc(data_801900fc);
    func_801575dc();
    func_801578ac();
    func_8015782c(sprite, 1, 0);
    func_8015786c();
    func_801656ec();
    func_80165790();
}
