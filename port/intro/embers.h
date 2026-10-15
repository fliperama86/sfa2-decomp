/* Private-pack adapter and bounded steady-scene pool. */
#ifndef PORT_INTRO_EMBERS_H
#define PORT_INTRO_EMBERS_H
#include <stddef.h>
#include "particles.h"
#include "sequence.h"
#define INTRO_EMBER_SLOTS 39u
#define INTRO_EMBER_TABLE_BYTES 608u
#define INTRO_EMBER_SEQUENCE_BYTES 612u
#define INTRO_EMBER_BYTES (INTRO_EMBER_TABLE_BYTES+INTRO_EMBER_SEQUENCE_BYTES+36u*256u)
struct intro_ember {
    struct intro_particle_motion motion;
    struct intro_sequence sequence;
    uint8_t state, subtype;
};
struct intro_embers {
    const uint8_t *data;
    struct intro_ember slots[INTRO_EMBER_SLOTS];
    uint16_t seed, interval, available;
    uint8_t free_slots[INTRO_EMBER_SLOTS];
};
/* Validates the entire extension before assigning state. Invalid input atomic. */
int intro_embers_init(struct intro_embers *s, const uint8_t *data, size_t size);
/* One ascending-slot update, then draw visible particles above the scene. */
int intro_embers_frame(struct intro_embers *s, uint16_t *pixels,
                        const uint16_t palette[16]);
#endif
