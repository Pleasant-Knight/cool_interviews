//#pragma once
#ifndef __LIBVERKADA__
#define __LIBVERKADA__

#ifdef __cplusplus
extern "C" { /* for inclusion from C++ */
#endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <dlfcn.h>
//#include "/usr/i686-w64-mingw32/sys-root/mingw/include/dlfcn.h"

/** 
 * Global const vars for the library.
*/
#define DYN_MEM_NO_HASH_TABLE_SIZE  (1 << 16)    /** 64K entries */
#define DYN_MEM_HASH_TABLE_KEY_MASK (0xFFFFFFFF) /** ((1 << 32) - 1) */
#define DYN_MEM_NO_SPACE_ALLOCS     (13)
#define DYN_MEM_NO_TIME_ALLOCS      (5)
#define DYN_MEM_NO_TIME_SECS        (1 << 10)    /** 1K seconds */
#define DYN_MEM_BYTES_TO_MB         (1 << 20)
#define DYN_MEM_MAX_COL_HIEGHT      (32)

/**
 * Hash table data structures.
 */
struct node {
    uintptr_t key;
    size_t val_sz;
    time_t val_ts;
    int valid;
    struct node* next;
};

struct hashtable {
    int size;
    struct node *list[DYN_MEM_NO_HASH_TABLE_SIZE];
};

struct _malloc_stats {
	/** Space Summary Stats: [0-4], [4-8], [8-16], [16-32], [32-64],
     * [64-128], [128-256], [256-512], [512-1024], [1024-2048], 
     * [2048-4096], [> 4096], total buckets.
     */
    long long int s_stats[DYN_MEM_NO_SPACE_ALLOCS];
	/** First timestamp. */
    time_t zero_ts; /** For debugging */
    /** To keep both space and time stats. */
    struct hashtable* ht;
};

/** Prints the current dynamic memory allocations/de-allocations. */
extern void print_dyn_mem_stats(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif/** __LIBVERKADA__ */
