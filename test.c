#include "arena.h"
#include "common.h"
#include <string.h>

size_t aligns[] = { 0, 2, 4, 8, 16 };
size_t aligns_size = sizeof (aligns) / sizeof (*aligns);

static void test_alloc (arena_t *pool, size_t size);
static void test_aligned_alloc (arena_t *pool, size_t size, size_t align);

static void
test1 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      test_alloc (&pool, size);
    }

  arena_free (&pool);
}

static void
test2 (void)
{
  arena_t pool = ARENA_INIT;
  size_t min_size = ARENA_BLOCK_SIZE;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (min_size, max_size);
      test_alloc (&pool, size);
    }

  arena_free (&pool);
}

static void
test3 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      test_alloc (&pool, size);
    }

  arena_free (&pool);
}

static void
test4 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      size_t align = rand_long (0, aligns_size);
      test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

static void
test5 (void)
{
  arena_t pool = ARENA_INIT;
  size_t min_size = ARENA_BLOCK_SIZE;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t align = rand_long (0, aligns_size);
      size_t size = rand_long (min_size, max_size);
      test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

static void
test6 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      size_t align = rand_long (0, aligns_size);
      test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

static void
test7 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      size_t align = rand_long (0, aligns_size);
      if (rand_long (0, 2))
        test_alloc (&pool, size);
      else
        test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

static void
test8 (void)
{
  arena_t pool = ARENA_INIT;
  size_t min_size = ARENA_BLOCK_SIZE;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t align = rand_long (0, aligns_size);
      size_t size = rand_long (min_size, max_size);
      if (rand_long (0, 2))
        test_alloc (&pool, size);
      else
        test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

static void
test9 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 10000; i++)
    {
      size_t size = rand_long (0, max_size);
      size_t align = rand_long (0, aligns_size);
      if (rand_long (0, 2))
        test_alloc (&pool, size);
      else
        test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

int
main (void)
{
  rand_init ();

  /* arena_alloc */
  test1 (); /* small */
  test2 (); /* big */
  test3 (); /* all */

  /* arena_aligned_alloc */
  test4 (); /* small */
  test5 (); /* big */
  test6 (); /* all */

  /* arena_alloc & arena_aligned_alloc */
  test7 (); /* small */
  test8 (); /* big */
  test9 (); /* all */
}

static void
test_alloc (arena_t *pool, size_t size)
{
  void *ptr = arena_alloc (pool, size);
  assert ((size && ptr) || (!size && !ptr));
  memset (ptr, 0, size);
}

static void
test_aligned_alloc (arena_t *pool, size_t size, size_t align)
{
  void *ptr = arena_aligned_alloc (pool, size, align);
  assert ((size && ptr) || (!size && !ptr));
  align = align ? align : ARENA_ALIGN_SIZE;
  assert ((size_t)ptr % align == 0);
  memset (ptr, 0, size);
}
