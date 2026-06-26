/**
 * @file light.h
 * @brief Lighting and material properties for illumination calculations
 *
 * This module defines light sources and material properties used in Gouraud
 * shading illumination model for rendering objects with realistic lighting.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include <GL/glew.h>

/**
 * @struct LightSource
 * @brief Represents a point light source in 3D space
 *
 * @var LightSource::x
 *      X coordinate of the light position
 * @var LightSource::y
 *      Y coordinate of the light position
 * @var LightSource::z
 *      Z coordinate of the light position
 * @var LightSource::ambient
 *      Ambient light color and intensity (RGBA)
 * @var LightSource::diffuse
 *      Diffuse light color and intensity (RGBA)
 * @var LightSource::specular
 *      Specular light color and intensity (RGBA)
 */
typedef struct LightSource
{
  GLfloat x;
  GLfloat y;
  GLfloat z;

  float ambient[4];
  float diffuse[4];
  float specular[4];
} LightSource;

/**
 * @struct MaterialLight
 * @brief Represents material properties for lighting calculations
 *
 * @var MaterialLight::emissive
 *      Emissive color (RGBA) - light emitted by the material
 * @var MaterialLight::ambient
 *      Ambient reflectance color (RGBA)
 * @var MaterialLight::diffuse
 *      Diffuse reflectance color (RGBA)
 * @var MaterialLight::specular
 *      Specular reflectance color (RGBA)
 * @var MaterialLight::shininess
 *      Specular shininess exponent, higher values create sharper highlights
 */
typedef struct MaterialLight
{
  float emissive[4];
  float ambient[4];
  float diffuse[4];
  float specular[4];
  float shininess;
} MaterialLight;

/**
 * @brief Apply Gouraud shading illumination model to calculate vertex lighting
 *
 * Implements the Gouraud shading technique to compute per-vertex lighting,
 * which is then interpolated across the surface. Calculates ambient, diffuse,
 * and specular components of illumination.
 *
 * @param lightDir
 *        Direction vector to the light source (normalized 3D vector)
 * @param modelViewMatrix
 *        Model-view transformation matrix for transforming coordinates
 */
void gouraudLightning (const GLfloat *lightDir,
                       const GLfloat *modelViewMatrix);

#endif // LIGHT_H
