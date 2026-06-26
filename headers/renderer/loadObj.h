/**
 * @file loadObj.h
 * @brief OBJ mesh file loading interface
 *
 * Provides functionality to load and parse Wavefront OBJ mesh files.
 */

#ifndef LOAD_OBJ_H
#define LOAD_OBJ_H

#include <stddef.h>

/**
 * @brief Load mesh geometry from an OBJ file
 *
 * Parses a Wavefront OBJ file and converts it to GPU-ready vertex data.
 * Returns an interleaved array of vertex attributes: position (3 floats),
 * texture coordinates (2 floats), normal (3 floats) per vertex.
 *
 * @param location Path to the OBJ mesh file
 * @param vertexCount Pointer to store the total number of processed vertices
 * @return Pointer to interleaved vertex data array (8 floats per vertex),
 *         or NULL if loading failed. Caller must free the returned pointer.
 */
float *loadObj (const char *location, size_t *vertexCount);

#endif // LOAD_OBJ_H
