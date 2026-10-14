/* Project-authored CPS2 intro sequence API. No executable ROM addresses. */
#ifndef PORT_INTRO_SEQUENCE_H
#define PORT_INTRO_SEQUENCE_H
#include <stddef.h>
#include <stdint.h>

struct intro_sequence {
    uint32_t record;
    uint16_t remaining, flags;
};

/* ROM maps CPU address zero, at most 16 MiB. State and ROM must not alias.
 * Return 0 on success, -1 on malformed input with state unchanged.
 * The selected record must have all 16 bytes; graphics are not read here. */
int intro_sequence_select(const uint8_t *rom, size_t size, uint32_t table,
                          uint32_t index, struct intro_sequence *state);
/* Call once per animation update, not once per host render. A zero countdown
 * wraps to 65535. While it remains nonzero, no ROM read is needed. */
int intro_sequence_tick(const uint8_t *rom, size_t size,
                        struct intro_sequence *state);
#endif
