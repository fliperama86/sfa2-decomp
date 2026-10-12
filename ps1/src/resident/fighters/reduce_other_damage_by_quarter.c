/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void reduce_other_damage_by_quarter(void) {
    ref_other.p->damage = ref_other.p->damage - (ref_other.p->damage >> 2);
}

void reduce_other_damage_by_eighth(void) {
    ref_other.p->damage = ref_other.p->damage - (ref_other.p->damage >> 3);
}

void noop_empty_function_11(void) {
}

void increase_other_damage_by_quarter(void) {
    ref_other.p->damage = ref_other.p->damage + (ref_other.p->damage >> 2);
}

void increase_other_damage_by_half(void) {
    ref_other.p->damage = ref_other.p->damage + (ref_other.p->damage >> 1);
}
