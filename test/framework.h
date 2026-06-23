#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <GLFW/glfw3.h>

/**
 * Signatur einer Testfunktion: keine Parameterübergabe und boolsche Rückgabe
 */
typedef int (*test_fn) (void);

/**
 * Führt eine Testfunktion aus und schreibt das Ergebnis auf die
 * Standartausgabe.
 */
void run_test (const char *name, test_fn fn);

/**
 * Hilfsfunktion zum vergleichen von zwei GLfloats unter Beachtung einer
 * Epsilon-Abweichung.
 */
int float_equal (GLfloat a, GLfloat b);

/**
 * Hilfsfunktion zum vergleichen von zwei Vektoren unter Beachtung einer
 * Epsilon-Abweichung.
 */
int vector_equal (const GLfloat *a, const GLfloat *b);

/**
 * Hilfsfunktion zum vergleichen von zwei Matrizen unter Beachtung einer
 * Epsilon-Abweichung.
 */
int matrix_equal (const GLfloat *a, const GLfloat *b);

#endif // TEST_FRAMEWORK_H
