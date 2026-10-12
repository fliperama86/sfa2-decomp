/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectFn data_80015870_slot01[];
extern ObjectFn data_8001587c_slot01[];
void call_handler_by_mode_byte_0_or_1_slot01(void);

Poly28 *init_poly_ft4_run_with_tpage_clut_slot01(Object *obj, Poly28 *p, int a2, int a3) {
    int x = a2;
    int y = a3;
    u32 *d = (u32 *)obj->sequence->data_ptr;
    int i;
    int n;

    for (i = 0; i < 2; i++) {
        n = *(s16 *)d;
        do {
            func_8015c09c(p);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->tpage = x;
            p->clut = y;
            p++;
        } while (--n > 0);
    }
    return p;
}

void call_handler_by_state_0_by_table_bit_slot01(Object *obj) {
    if (obj->table_index & 0x80) {
        data_8001587c_slot01[obj->state_0](obj);
    } else {
        data_80015870_slot01[obj->state_0](obj);
        call_handler_by_mode_byte_0_or_1_slot01();
    }
}
