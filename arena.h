#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

#define ARENA_PAGE_SIZE 4096
#define ARENA_BLOCK_SIZE (8 * ARENA_PAGE_SIZE)
#define ARENA_ALIGN_SIZE (2 * sizeof (void *))

typedef struct arena_t arena_t;

struct arena_t
{
  void *pos;
  void *end;
  size_t remain;
  struct
  {
    size_t cap;
    size_t size;
    void **data;
  } blocks;
};

#define ARENA_INIT                                                            \
  (arena_t) {}

void arena_free (arena_t *pool);

void *arena_alloc (arena_t *pool, size_t size);

void *arnea_realloc (arena_t *pool, void *oldptr, size_t oldsiz,
                     size_t newsiz);

void *arena_aligned_alloc (arena_t *pool, size_t size, size_t align);

void *arena_aligned_realloc (arena_t *pool, void *oldptr, size_t oldsiz,
                             size_t newsiz, size_t align);

#endif
