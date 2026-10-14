## Owner rejects ember-only intro increment (2026-10-14)

After launching the new build, the owner reported that it was effectively the
same as before: only Akuma and flames. This corrects the implementation scope,
not the successful build or the narrowly scoped particle comparisons. The
increment added small moving embers but still returned after the same short
steady-scene slice; transitions and subsequent intro scenes were not delivered.

The assistant narrowed the planned fade/flash/ember work to an ember-only
increment, then offered it as the next test. That was insufficient progress
for the owner's expectation. Do not frame another cosmetic variation of the
same slice as a meaningful intro build. The next owner test must demonstrate
actual sequence progression, including transitions and subsequent scenes.
Recover those from code and tables, never visual imitation; keep all changes
port-only, preserve the default-off setting and normal executable, and provide
the meaningful runnable increment before a PR. No owner approval or PR is
claimed for the rejected test. Earlier evidence remains intact at its scope.
