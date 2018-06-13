/*
 * See Copyright Notice in qnode.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "qassert.h"
#include "qlog.h"

#ifdef DEBUG

void
qassert_error(char* expr, char* file, int line) {
  printf("[%s:%d] ASSERT %s error\n", file, line, expr);
  abort();
}

#endif
