/* Run independent jobs on the machine's processors: used where the loader builds many pieces from the arcade set at start. */
#ifndef PORT_PARALLEL_H
#define PORT_PARALLEL_H

/* Call job(context, i) once for each i below count, on up to one thread a processor, and return when all are done. The jobs
 * run in any order and at the same time: each must touch only its own data (and what no job changes). Without threads they
 * run here, one after another. */
void port_parallel(unsigned count, void (*job)(void *context, unsigned i), void *context);

#endif
