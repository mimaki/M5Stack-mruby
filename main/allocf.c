/*
** allocf.c - default memory allocation function
**
** See Copyright Notice in mruby.h
*/

#include <stdlib.h>
#include "mruby.h"

/* This function serves as the default memory allocation function and accepts four arguments:
 *
 * - `mrb`: An instance of `mrb_state`. It's important to note that for the initial allocation (used to allocate the `mrb_state` itself), `mrb` is set to NULL.
 * - `p`: The previous pointer to the memory region. For memory allocation, this parameter is NULL.
 * - `size`: The new size of the memory region to be returned.
 * - `ud`: User data, represented as a `void*`, which is passed to the `mrb_state`.
 */

// #define TRACE_ALLOC  // Out memory allocation trace log

void*
mrb_esp32_heap_allocf(mrb_state *mrb, void *p, size_t size, void *ud)
{
  void *pp = NULL;
#ifdef TRACE_ALLOC
  printf("//");   // Need `C` comment for compile mruby
#endif
  if (size == 0) {
    /* `free(NULL)` should be no-op */
#ifdef TRACE_ALLOC
    printf("%p,0x%04zx\n", p, size);
#endif
    free(p);
  }
  else {
#ifdef TRACE_ALLOC
    if (p) {
      printf("%p->", p);
    }
#endif
    /* `ralloc(NULL, size)` works as `malloc(size)` */
    pp = realloc(p, size);
#ifdef TRACE_ALLOC
    printf("%p,0x%04zx\n", pp, size);
#endif
  }
  return pp;
}
