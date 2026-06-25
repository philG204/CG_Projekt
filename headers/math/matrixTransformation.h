#ifndef MATRIX_TRANSFORMATION_H
#define MATRIX_TRANSFORMATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
 *  @brief Normalizes a 3D vector in-place
 *
 *  @param v Pointer to a 3D vector (modified in-place)
*/
void normalize (GLfloat *v);

/**
 *  @brief calculate the cross product of two 3D vector 
 *
 *  Performs out = a × b
 *
 *  @param out pointer to the output 3D vector
 *  @param a pointer to the input 3D vector 
 *  @param b pointer to the input 3D vector
*/
void cross (GLfloat *out, const GLfloat *a, const GLfloat *b);

/** 
 *  @brief Calculates the dot product(Skalarprodukt) of two 3D Vectors
 *
 *  Perform a · b
 *
 *  @param a pointer to the input 3D vector
 *  @param b pointer to the input 3D vector
 *  @return returns the dot product as GLFloat
*/
GLfloat dot (const GLfloat *a, const GLfloat *b);

/**
 *  @brief Multiplies two 4x4 matricies
 *
 *  @param out pointer to the output 4x4 Matrix
 *  @param a pointer to the first input 4x4 Matrix
 *  @param b pointer to the second input 4x4 Matrix
*/
void multiplyMatrices (GLfloat *out, const GLfloat *a, const GLfloat *b);

/**
 *  @brief set a 4x4 matrix a to the identity matrix
 *
 *  @param out pointer to the 4x4 Matrix that should be set to the identity matrix
*/
void identity (GLfloat *out);

/**
 *  @brief Applies a translation transformation to a 4x4 matrix
 *
 *  @param out Pointer to the resulting 4x4 matrix
 *  @param in Pointer to the input 4x4 matrix
 *  @param v Pointer to a 3-component translation vector
*/
void translate (GLfloat *out, const GLfloat *in, GLfloat *v);

/**
 *  @brief Rotates a 4x4 matrix around the X axis 
 *
 *  @param out Pointer to the output 4x4 Matrix
 *  @param in Pointer to the input 4x4 Matrix
 *  @param angle Rotation angle in radians
*/
void rotatex (GLfloat *out, const GLfloat *in, GLfloat angle);

/**
 *  @brief Rotates a 4x4 matrix around the Y axis 
 *
 *  @param out Pointer to the output 4x4 Matrix
 *  @param in Pointer to the input 4x4 Matrix
 *  @param angle Rotation angle in radians
*/
void rotatey (GLfloat *out, const GLfloat *in, GLfloat angle);

/**
 *  @brief Rotates a 4x4 matrix around the Z axis 
 *
 *  @param out Pointer to the output 4x4 Matrix
 *  @param in Pointer to the input 4x4 Matrix
 *  @param angle Rotation angle in radians
*/
void rotatez (GLfloat *out, const GLfloat *in, GLfloat angle);

/**
 *  @brief Applies a scaling transformation to a 4x4 matrix
 *
 *  @param out Pointer to the output 4x4 Matrix
 *  @param in Pointer to the input 4x4 Matrix
 *  @param v Scaling Factors for x, y, z
*/
void scale (GLfloat *out, const GLfloat *in, const GLfloat *v);

/**
 *  @brief Creates a view matrix using eye, center and up
 *
 *  @param out Pointer to the resulting 4x4 view Matrix
 *  @param eye Pointer to the Camara as 3D vector
 *  @param center Pointer to the Target point the camera looks at, as 3D vector 
 *  @param up Pointer to the Up direction vector, as 3D vector
*/
void lookAt (GLfloat *out, const GLfloat *eye, const GLfloat *center,
             const GLfloat *up);

/**
 *  @brief Creates a perspective projection matrix
 *
 *  @param out Pointer to the resulting 4x4 projection Matrix
 *  @param fovy Field of view in radians
 *  @param aspect Aspect ratio (width / height)
 *  @param near Near clipping plane
 *  @param far Far clipping plane
*/
void perspective (GLfloat *out, float fovy, float aspect, float near,
                  float far);

#endif // MATRIX_TRANSFORMATION_H
