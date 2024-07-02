#include "arena.h"
#include <stdbool.h>
#include <stdlib.h>

static void *alloc (arena_t *pool, size_t size);

#define block_alloc(pool) alloc (pool, ARENA_BLOCK_SIZE)

#define huge_alloc(pool, size)                                                \
  ({                                                                          \
    void *ptr;                                                                \
    if ((ptr = alloc (pool, size)))                                           \
      pool->remain = 0;                                                       \
    ptr;                                                                      \
  })

void
arena_free (arena_t *pool)
{
  for (size_t i = 0; i < pool->blocks.size; i++)
    free (pool->blocks.data[i]);
  free (pool->blocks.data);
  *pool = ARENA_INIT;
}

void *
arena_alloc (arena_t *pool, size_t size)
{
  if (!size)
    return NULL;

  if (size >= ARENA_BLOCK_SIZE)
    return huge_alloc (pool, size);

  if (size > pool->remain && !block_alloc (pool))
    return NULL;

  void *ret = pool->pos;
  pool->remain -= size;
  pool->pos += size;
  return ret;
}

void *
arena_aligned_alloc (arena_t *pool, size_t size, size_t align)
{
  if (!size)
    return NULL;

  if (!align)
    align = ARENA_ALIGN_SIZE;

  if (size >= ARENA_BLOCK_SIZE)
    return huge_alloc (pool, size);

  size_t padding = align - (size_t)pool->pos % align;
  padding = (padding == align) ? 0 : padding;
  void *ret = pool->pos + padding;

  if (size + padding <= pool->remain)
    size += padding;
  else if (block_alloc (pool))
    ret = pool->pos;
  else
    return NULL;

  pool->remain -= size;
  pool->pos += size;
  return ret;
}

static inline void *
alloc (arena_t *pool, size_t size)
{
  void *block;
  void **data = pool->blocks.data;

  if (!(block = malloc (size)))
    return NULL;

  if (pool->blocks.size >= pool->blocks.cap)
    {
      size_t cap = pool->blocks.cap;
      cap = cap ? cap * 2 : 8;

      if (!(data = realloc (data, cap * sizeof (void *))))
        {
          free (block);
          return NULL;
        }

      pool->blocks.data = data;
      pool->blocks.cap = cap;
    }

  data[pool->blocks.size++] = block;
  pool->remain = size;
  pool->pos = block;
  return block;
}
