/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80140f00(void) {
    ref_other.p->damage = ref_other.p->damage - (ref_other.p->damage >> 2);
}

void func_80140f24(void) {
    ref_other.p->damage = ref_other.p->damage - (ref_other.p->damage >> 3);
}

void func_80140f48(void) {
}

void func_80140f50(void) {
    ref_other.p->damage = ref_other.p->damage + (ref_other.p->damage >> 2);
}

void func_80140f74(void) {
    ref_other.p->damage = ref_other.p->damage + (ref_other.p->damage >> 1);
}
