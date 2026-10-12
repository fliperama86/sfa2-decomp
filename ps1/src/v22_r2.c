#include "game.h"
#include "externs.h"
#include "protos.h"

void init_snd_voices(void) {
    int base = *(int *)0x110;
    SndVoice *v = (SndVoice *)0x801fc200;
    int i;
    for (i = 0; i < 3; i++) {
        base += 0xc0;
        v->status = 0;
        v->stack_address = 0x801fec00 + i * 0x800;
        *(int *)(base + 0x94) = 0x40000404;
        v++;
    }
}
