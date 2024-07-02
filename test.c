#include "arena.h"
#include "common.h"
#include <string.h>

static void test_alloc (arena_t *pool, size_t size);
static void test_aligned_alloc (arena_t *pool, size_t size, size_t align);

static void
test0 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;

  for (size_t i = 0; i < 1000; i++)
    {
      size_t size = rand_long (0, max_size);
      test_alloc (&pool, size);
    }

  arena_free (&pool);
}

static void
test1 (void)
{
  arena_t pool = ARENA_INIT;
  size_t max_size = ARENA_BLOCK_SIZE * 2 + 1;
  size_t aligns[] = { 0, 2, 4, 8, 16, 32, 64 };
  size_t aligns_size = sizeof (aligns) / sizeof (*aligns);

  for (size_t i = 0; i < 1000; i++)
    {
      size_t size = rand_long (0, max_size);
      size_t align = rand_long (0, aligns_size);
      test_aligned_alloc (&pool, size, aligns[align]);
    }

  arena_free (&pool);
}

int
main (void)
{
  rand_init ();
  test0 ();
  test1 ();
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
