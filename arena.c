#include "arena.h"
#include <stdbool.h>
#include <stdlib.h>

void
arena_free (arena_t *pool)
{
  for (size_t i = 0; i < pool->blocks.size; i++)
    free (pool->blocks.data[i]);
  free (pool->blocks.data);
  *pool = ARENA_INIT;
}

bool
block_alloc (arena_t *pool)
{
  void *block;

  if (!(block = malloc (ARENA_BLOCK_SIZE)))
    return false;

  if (pool->blocks.size >= pool->blocks.cap)
    {
      void **newdata;
      size_t cap = pool->blocks.cap;
      void *data = pool->blocks.data;
      size_t newcap = cap ? cap * 2 : 8;

      if (!(newdata = realloc (data, newcap * sizeof (void *))))
        {
          free (block);
          return false;
        }

      pool->blocks.data = newdata;
      pool->blocks.cap = newcap;
    }

  pool->blocks.data[pool->blocks.size++] = block;
  pool->remain = ARENA_BLOCK_SIZE;
  pool->pos = block;
  return true;
}

void *
huge_alloc (arena_t *pool, size_t size)
{
  void *block;

  if (!(block = malloc (size)))
    return false;

  if (pool->blocks.size >= pool->blocks.cap)
    {
      void **newdata;
      size_t cap = pool->blocks.cap;
      void *data = pool->blocks.data;
      size_t newcap = cap ? cap * 2 : 8;

      if (!(newdata = realloc (data, newcap * sizeof (void *))))
        {
          free (block);
          return false;
        }

      pool->blocks.data = newdata;
      pool->blocks.cap = newcap;
    }

  pool->blocks.data[pool->blocks.size++] = block;
  pool->remain = 0;
  return block;
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
