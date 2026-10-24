#include "parallel.h"
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#define MAX_THREADS 32
#define STACK_BYTES (8u << 20)   /* each helper's stack: as the main thread's on Linux, where the builds are tested */

struct work {
    void (*job)(void *, unsigned);
    void *context;
    unsigned count, next;   /* next: the first job no thread has taken (atomic) */
};

static void take(struct work *w)
{
    unsigned i;
    while ((i = __atomic_fetch_add(&w->next, 1u, __ATOMIC_RELAXED)) < w->count) w->job(w->context, i);
}

static unsigned processors(void)
{
#ifdef _WIN32
    SYSTEM_INFO s;
    GetSystemInfo(&s);
    return s.dwNumberOfProcessors;
#else
    long n = sysconf(_SC_NPROCESSORS_ONLN);
    return n > 0 ? (unsigned)n : 1u;
#endif
}

#ifdef _WIN32
static DWORD WINAPI helper(LPVOID w)
{
    take(w);
    return 0;
}
#else
static void *helper(void *w)
{
    take(w);
    return NULL;
}
#endif

void port_parallel(unsigned count, void (*job)(void *context, unsigned i), void *context)
{
    struct work w;
    unsigned n = processors(), started = 0, i;
#ifdef _WIN32
    HANDLE t[MAX_THREADS];
#else
    pthread_t t[MAX_THREADS];
    pthread_attr_t attr;
    int attr_ok = pthread_attr_init(&attr) == 0;
    if (attr_ok) pthread_attr_setstacksize(&attr, STACK_BYTES);
#endif
    w.job = job;
    w.context = context;
    w.count = count;
    w.next = 0;
    if (n > MAX_THREADS) n = MAX_THREADS;
    if (n > count) n = count;
    for (i = 1; i < n; i++) {   /* this thread is the first */
#ifdef _WIN32
        if (!(t[started] = CreateThread(NULL, STACK_BYTES, helper, &w, STACK_SIZE_PARAM_IS_A_RESERVATION, NULL))) break;
#else
        if (pthread_create(&t[started], attr_ok ? &attr : NULL, helper, &w) != 0) break;
#endif
        started++;
    }
    take(&w);
    for (i = 0; i < started; i++) {
#ifdef _WIN32
        WaitForSingleObject(t[i], INFINITE);
        CloseHandle(t[i]);
#else
        pthread_join(t[i], NULL);
#endif
    }
#ifndef _WIN32
    if (attr_ok) pthread_attr_destroy(&attr);
#endif
}
