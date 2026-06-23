#include "framework.h"
#include <math.h>
#include <stdio.h>

#define EPSILON 0.00001f

int
float_equal (GLfloat a, GLfloat b)
{
  return fabsf (a - b) < EPSILON;
}

// Da sowohl Vektoren als auch Matrizen kontinuierlich im Speicher liegen
// können wir einen Array-Vergleich ausführen
static int
array_equal (const GLfloat *a, const GLfloat *b, int n)
{
  for (int i = 0; i < n; i++)
    {
      if (!float_equal (a[i], b[i]))
        return 0;
    }

  return 1;
}

int
vector_equal (const GLfloat *a, const GLfloat *b)
{
  return array_equal (a, b, 3);
}

int
matrix_equal (const GLfloat *a, const GLfloat *b)
{
  return array_equal (a, b, 16);
}

void
run_test (const char *name, test_fn fn)
{
  if (fn ())
    printf ("%s: Ok\n", name);
  else
    printf ("%s: Nicht Ok\n", name);
}
