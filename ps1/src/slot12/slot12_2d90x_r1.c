/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801ae02d;
extern u8 data_801ae02e;
extern void (*data_80023170_slot12[])(Object *);
Object *func_8011f1e0(void);
void func_8001281c_slot12(Object *obj);

int func_80012d90_slot12(u8 arg) {
    Object *v;
    int r;
    u8 *flag = &data_801ae02c;

    r = 0;
    if (*flag == 0) {
        *flag = 1;
        data_801ae02e = arg;
        data_801ae02d = 0;
        v = func_8011f1e0();
        if (v != 0) {
            v->field_00 = 1;
            r = 1;
            v->field_02 = 0x7e;
            v->field_03 = arg + 0x80;
        }
    }
    func_8001281c_slot12(v);
    return r;
}
