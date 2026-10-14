/* Project-authored reconstruction of the CPS2 sequence selector/tick.
 * Behavioral C, not a byte match. See sequence-contract.md and its fixtures.
 * No game bytes, tables, decompiler output or CPU interpreter are embedded. */
#include "sequence.h"

static uint16_t be16(const uint8_t *p)
{
    return (uint16_t)((uint16_t)p[0] << 8 | p[1]);
}

static int span(size_t size, uint32_t at, size_t count)
{
    return size <= 0x1000000u && !(at & 1u) && at <= size && count <= size - at;
}

static int load(const uint8_t *rom, size_t size, uint32_t record,
                struct intro_sequence *state)
{
    if (!rom || !span(size, record, 16)) return -1;
    state->record = record;
    state->remaining = be16(rom + record);
    state->flags = be16(rom + record + 2);
    return 0;
}

int intro_sequence_select(const uint8_t *rom, size_t size, uint32_t table,
                          uint32_t index, struct intro_sequence *state)
{
    uint32_t entry, word;
    int32_t displacement;
    int64_t record;
    if (!state || !rom || !span(size, table, 2)) return -1;
    entry = table + (index & 255u) * 2u;
    if (!span(size, entry, 2)) return -1;
    word = be16(rom + entry);
    displacement = word < 0x8000u ? (int32_t)word : (int32_t)word - 0x10000;
    record = (int64_t)table + displacement;
    if (record < 0 || record > 0xffffff) return -1;
    return load(rom, size, (uint32_t)record, state);
}

int intro_sequence_tick(const uint8_t *rom, size_t size,
                        struct intro_sequence *state)
{
    uint16_t remaining;
    uint32_t next;
    if (!state) return -1;
    remaining = (uint16_t)(state->remaining - 1u);
    if (remaining) {
        state->remaining = remaining;
        return 0;
    }
    if (!rom || !span(size, state->record, 16)) return -1;
    next = state->record + 16u;
    if (state->flags & 0x8000u) {
        if (!span(size, next, 4)) return -1;
        next = (uint32_t)be16(rom + next) << 16 | be16(rom + next + 2);
    }
    return load(rom, size, next, state);
}
