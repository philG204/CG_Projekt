#include <GL/glew.h>
#include <math.h>

#include "../headers/math/matrixTransformation.h"
#include "framework.h"
#include "test_entries.h"

/**
  @brief

  @return
*/
int
test_normalize_vector (void)
{
  GLfloat v[3] = { 3.0f, 4.0f, 0.0f };

  normalize (v);

  GLfloat expected[3] = { 0.6f, 0.8f, 0.0f };

  return vector_equal (v, expected);
}

/**
  @brief

  @return
*/
int
test_normalize_zero_vector (void)
{
  GLfloat v[3] = { 0.0f, 0.0f, 0.0f };

  normalize (v);

  GLfloat expected[3] = { 0.0f, 0.0f, 0.0f };

  return vector_equal (v, expected);
}

/**
  @brief test the cross product a × b

  @return
*/
int
test_cross_product (void)
{
  GLfloat a[3] = { 1.0f, 0.0f, 0.0f };
  GLfloat b[3] = { 0.0f, 1.0f, 0.0f };
  GLfloat out[3];

  cross (out, a, b);

  GLfloat expected[3] = { 0.0f, 0.0f, 1.0f };

  return vector_equal (out, expected);
}

/**
  @brief test the dot function(Skalarprodukt)

  @return
*/
int
test_dot_product (void)
{
  GLfloat a[3] = { 1, 2, 3 };
  GLfloat b[3] = { 4, 5, 6 };

  return float_equal (dot (a, b), 32.0f);
}

/**
  @brief test the identity function

  @return
*/
int
test_identity (void)
{
  GLfloat m[16];

  identity (m);

  GLfloat expected[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

  return matrix_equal (m, expected);
}

/**
  @brief

  @return
*/
int
test_multiply_identity (void)
{
  GLfloat a[16];
  GLfloat b[16];
  GLfloat out[16];

  identity (a);
  identity (b);

  multiplyMatrices (out, a, b);

  return matrix_equal (out, a);
}

/**
  @brief

  @return
*/
int
test_multiply_alias_output_left_operand (void)
{
  GLfloat a[16];
  GLfloat b[16];

  identity (a);
  identity (b);

  b[12] = 3.0f;

  multiplyMatrices (a, a, b);

  return float_equal (a[12], 3.0f);
}

/**
  @brief 

  @return
*/
int
test_multiply_alias_output_right_operand (void)
{
  GLfloat a[16];
  GLfloat b[16];

  identity (a);
  identity (b);

  b[12] = 7.0f;

  multiplyMatrices (b, a, b);

  return float_equal (b[12], 7.0f);
}

/**
  @brief

  @return
*/
int
test_translate_identity (void)
{
  GLfloat in[16];
  GLfloat out[16];
  GLfloat v[3] = { 1.0f, 2.0f, 3.0f };

  identity (in);

  translate (out, in, v);

  return float_equal (out[12], 1.0f) && float_equal (out[13], 2.0f)
         && float_equal (out[14], 3.0f) && float_equal (out[15], 1.0f);
}

/**
  @brief 

  @return
*/
int
test_translate_in_place (void)
{
  GLfloat m[16];
  GLfloat v[3] = { 5.0f, 6.0f, 7.0f };

  identity (m);

  translate (m, m, v);

  return float_equal (m[12], 5.0f) && float_equal (m[13], 6.0f)
         && float_equal (m[14], 7.0f);
}

/**
  @brief checks the rotateX function

  @return
*/
int
test_rotatex_90deg (void)
{
  GLfloat m[16];

  identity (m);

  rotatex (m, m, (GLfloat)(M_PI / 2.0));

  return float_equal (m[5], 0.0f) && float_equal (m[6], 1.0f)
         && float_equal (m[9], -1.0f) && float_equal (m[10], 0.0f);
}

/**
  @brief checks the rotatey function

  @return
*/
int
test_rotatey_90deg (void)
{
  GLfloat m[16];

  identity (m);

  rotatey (m, m, (GLfloat)(M_PI / 2.0));

  return float_equal (m[0], 0.0f) && float_equal (m[2], -1.0f)
         && float_equal (m[8], 1.0f) && float_equal (m[10], 0.0f);
}

/**
  @brief checks the rotatez function

  @return
*/
int
test_rotatez_90deg (void)
{
  GLfloat m[16];

  identity (m);

  rotatez (m, m, (GLfloat)(M_PI / 2.0));

  return float_equal (m[0], 0.0f) && float_equal (m[1], 1.0f)
         && float_equal (m[4], -1.0f) && float_equal (m[5], 0.0f);
}

/**
  @brief checks the scale function

  @return
*/
int
test_scale (void)
{
  GLfloat m[16];
  GLfloat v[3] = { 2.0f, 3.0f, 4.0f };

  identity (m);

  scale (m, m, v);

  return float_equal (m[0], 2.0f) && float_equal (m[5], 3.0f)
         && float_equal (m[10], 4.0f) && float_equal (m[15], 1.0f);
}

/**
  @brief checks looksAt Methode

  @return
*/
int
test_lookat_origin (void)
{
  GLfloat out[16];

  GLfloat eye[3] = { 0.0f, 0.0f, 0.0f };
  GLfloat center[3] = { 0.0f, 0.0f, -1.0f };
  GLfloat up[3] = { 0.0f, 1.0f, 0.0f };

  lookAt (out, eye, center, up);

  GLfloat expected[16];

  identity (expected);

  return matrix_equal (out, expected);
}

/**
  @brief checks the the calculation for the perspective

  @return
*/
int
test_perspective_basic (void)
{
  GLfloat m[16];

  perspective (m, (GLfloat)(M_PI / 2.0f), 1.0f, 1.0f, 100.0f);

  return float_equal (m[0], 1.0f) && float_equal (m[5], 1.0f)
         && float_equal (m[11], -1.0f) && float_equal (m[10], -101.0f / 99.0f)
         && float_equal (m[14], -200.0f / 99.0f);
}

/**
  @brief test all the Test one after the another

  @return
*/
void
test_matrixTransormation (void)
{
  run_test ("normalize vector", test_normalize_vector);
  run_test ("normalize zero vector", test_normalize_zero_vector);

  run_test ("cross product", test_cross_product);
  run_test ("dot product", test_dot_product);

  run_test ("identity", test_identity);

  run_test ("multiply identity", test_multiply_identity);
  run_test ("multiply alias left", test_multiply_alias_output_left_operand);
  run_test ("multiply alias right", test_multiply_alias_output_right_operand);

  run_test ("translate", test_translate_identity);
  run_test ("translate in place", test_translate_in_place);

  run_test ("rotatex 90deg", test_rotatex_90deg);
  run_test ("rotatey 90deg", test_rotatey_90deg);
  run_test ("rotatez 90deg", test_rotatez_90deg);

  run_test ("scale", test_scale);

  run_test ("lookAt canonical", test_lookat_origin);

  run_test ("perspective", test_perspective_basic);
}
