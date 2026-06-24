#include "framework.h"
#include <math.h>
#include <stdio.h>

#define EPSILON 0.00001f

/**
 *  @brief
 *
 *  @param a
 *  @param b
 *  @return
*/
int
float_equal (GLfloat a, GLfloat b)
{
  return fabsf (a - b) < EPSILON;
}

/**
 *  @brief Da sowohl Vektoren als auch Matrizen kontinuierlich im Speicher liegen können wir einen Array-Vergleich ausführen
 *
 *  @param a
 *  @param b
 *  @param n
 *  @return
*/ 
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

/**
 *  @brief
 *
 *  @param a
 *  @param b
 *  @return
*/
int
vector_equal (const GLfloat *a, const GLfloat *b)
{
  return array_equal (a, b, 3);
}

/**
 *  @brief
 *
 *  @param a
 *  @param b
 *  @return
*/
int
matrix_equal (const GLfloat *a, const GLfloat *b)
{
  return array_equal (a, b, 16);
}

/**
 *  @brief
 *
 *  @param name
 *  @param fn
*/
void
run_test (const char *name, test_fn fn)
{
  if (fn ())
    printf ("%s: Ok\n", name);
  else
    printf ("%s: Nicht Ok\n", name);
}
