/* Native reconstruction, scoped contract: particles-contract.md. */
#ifndef PORT_INTRO_PARTICLES_H
#define PORT_INTRO_PARTICLES_H
#include <stdint.h>
struct intro_particle_motion {
    uint32_t x, y, vx, vy, sway, acceleration, limit;
};
struct intro_particle_spawn {
    uint16_t subtype, layer, priority;
};
uint8_t intro_particle_random(uint16_t *seed);
int intro_particle_spawn(uint16_t *interval, uint16_t *seed, unsigned phase,
                         unsigned available, struct intro_particle_spawn *out);
/* Caller supplies immutable, nonaliasing tables: 96 velocity + 512 spawn bytes. */
int intro_particle_init(struct intro_particle_motion *p, unsigned subtype,
                        uint16_t *seed, const uint8_t *tables);
/* Returns one when bounds request deferred deletion, otherwise zero. */
int intro_particle_move(struct intro_particle_motion *p);
#endif
