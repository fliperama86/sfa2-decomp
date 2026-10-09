/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015cec4(int a, u8 *p);
int func_8015c990(int a);
int func_8015c958(int a, u8 *p);

Node *func_8014f194(int index) {
    s16 arg = index;
    Menu *m = &data_80190948;
    u8 buf[4];
    int s2;
    int n;
    u8 *q;
    buf[0] = 0x80;
    while (!func_8015cc44(0xe, (int)buf, 0)) {
    }
    m->field_28 = 0x10;
    func_8015cec4(0x10, &m->field_f0);
    while (!func_8015c9c0(0x15, &m->field_f0, 0)) {
    }
    do {
        while (!func_8015cc44(1, 0, (int)&m->field_0c)) {
        }
    } while (m->field_0c & 0x40);
    while (!func_8015cc44(9, 0, 0)) {
    }
    func_8015c990(0);
    m->field_02 = 0;
    m->field_00 = 1;
    m->field_01 = 0;
    m->field_0a = 0;
    func_80150c6c(m, arg);
    func_8015cec4(m->field_28, &m->field_f0);
again:
    {
        s2 = 0;
        while (!func_8015cc44(0xe, (int)buf, 0)) {
        }
        while (!func_8015cc44(2, (int)&m->field_f0, 0)) {
        }
        while (!func_8015cc44(6, 0, 0)) {
        }
        do {
            n = func_8015c958(1, &m->field_0c);
        } while (n == 0);
        if (n == 5) {
            goto again;
        }
        func_8015cdbc((u8 *)0x801e0000, 0x200);
        if (m->field_34 != *(u32 *)0x801e0000) {
            goto again;
        }
        q = (u8 *)0x801e0000;
        n = *(u32 *)0x801e001c >> 11;
        m->field_38 = (u8 *)*(u32 *)0x801e0018;
        for (; n > 0; n--) {
            if (func_8015c958(0, &m->field_0c) == 5) {
                s2++;
                break;
            }
            func_8015cdbc(m->field_38, 0x200);
            m->field_38 = m->field_38 + 0x800;
        }
        if (s2 > 0) {
            goto again;
        }
    }
    while (!func_8015cc44(9, 0, 0)) {
    }
    return (Node *)(q + 0x10);
}
