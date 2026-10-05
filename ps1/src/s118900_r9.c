/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011a55c(Object *o) {
    int i = o->field_03;
    int j = o->field_3c->side;
    data_80185c84[j][i][0] = 0;
    data_80185cc4[j][i][0] = 0;
    data_80185ce4[j][i][0] = 0;
    data_80185c84[j][i][1] = 0;
    data_80185cc4[j][i][1] = 0;
    data_80185ce4[j][i][1] = 0;
}

u16 func_8011a5f4(void) {
    return data_801846fc;
}

u16 func_8011a604(int a, u16 b) {
    int head;
    int cur;
    int i;
    if (data_80184700 == 0) {
        return 0xffff;
    }
    cur = (u16)a;
    head = data_801846fc;
    data_801846fc = data_80183c90[head];
    for (i = 0; i < b; i++) {
        cur = data_80183c90[cur];
    }
    data_80183c90[cur] = head;
    data_80184700--;
    return head;
}

void func_8011a6b8(u16 a, u16 b) {
    int cur;
    u32 i;
    if (b != 0) {
        cur = a;
        for (i = 0; i < b - 1; i++) {
            cur = data_80183c90[cur];
        }
        data_80183c90[cur] = data_801846fc;
        data_801846fc = a;
        data_80184700 += b;
    }
}

void func_8011a744(void) {
    func_8011a018();
    func_8011a410();
    func_801511c8();
    func_801518a0();
}
