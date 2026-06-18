#include "framework.h"
#include <math.h>
#include <stdio.h>

#define EPSILON 0.00001f

int
float_equal (GLfloat a, GLfloat b)
{
  return fabsf (a - b) < EPSILON;
}

int
matrix_equal (const GLfloat *a, const GLfloat *b)
{
  for (int i = 0; i < 16; i++)
    {
      if (!float_equal (a[i], b[i]))
        return 0;
    }

  return 1;
}

void
run_test (const char *name, test_fn fn)
{
  if (fn ())
    printf ("%s: Ok\n", name);
  else
    printf ("%s: Nicht Ok\n", name);
}
