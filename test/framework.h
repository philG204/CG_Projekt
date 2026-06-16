#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <GLFW/glfw3.h>

typedef int (*test_fn) (void);

void run_test (const char *name, test_fn fn);
int float_equal (GLfloat a, GLfloat b);
int matrix_equal (const GLfloat *a, const GLfloat *b);

#endif // TEST_FRAMEWORK_H
