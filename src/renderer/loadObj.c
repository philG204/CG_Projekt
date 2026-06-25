#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../headers/renderer/loadObj.h"

/** @brief */
typedef float Vec2[2];
/** @brief */
typedef float Vec3[3];

/**
 *  @brief
 *
 *  @param v
 *  @param vt
 *  @param vn
 */
typedef struct FaceIndex
{
  int v;
  int vt;
  int vn;
} FaceIndex;

/**
 *  @brief
 *
 *  @param token
 *  @param out
 *  @return
 */
static int
parseFaceToken (const char *token, FaceIndex *out)
{
  out->v = 0;
  out->vt = 0;
  out->vn = 0;

  // Format: v/vt/vn
  if (sscanf (token, "%d/%d/%d", &out->v, &out->vt, &out->vn) == 3)
    {
      return 1;
    }

  // Format: v//vn
  if (sscanf (token, "%d//%d", &out->v, &out->vn) == 2)
    {
      out->vt = 0;
      return 1;
    }

  // Format: v/vt
  if (sscanf (token, "%d/%d", &out->v, &out->vt) == 2)
    {
      out->vn = 0;
      return 1;
    }

  // Format: v
  if (sscanf (token, "%d", &out->v) == 1)
    {
      out->vt = 0;
      out->vn = 0;
      return 1;
    }

  return 0;
}

/**
 *  @brief
 *
 *  @param index
 *  @param count
 *  @return
 */
static int
validIndex (int index, size_t count)
{
  return index > 0 && (size_t)index <= count;
}

float *
loadObj (const char *location, size_t *vertexCount)
{
  if (vertexCount == NULL)
    {
      fprintf (stderr, "loadObj: vertexCount ist NULL\n");
      return NULL;
    }

  *vertexCount = 0;

  if (location == NULL)
    {
      fprintf (stderr, "loadObj: location ist NULL\n");
      return NULL;
    }

  FILE *file = fopen (location, "r");
  if (file == NULL)
    {
      perror ("fopen");
      return NULL;
    }

  printf ("OBJ-Datei geoeffnet: %s\n", location);

  char line[1024];

  size_t vCount = 0;
  size_t vtCount = 0;
  size_t vnCount = 0;
  size_t fCount = 0;

  // 1. Durchlauf: Anzahl der Daten zählen
  while (fgets (line, sizeof (line), file))
    {
      if (strncmp (line, "v ", 2) == 0)
        {
          ++vCount;
        }
      else if (strncmp (line, "vt ", 3) == 0)
        {
          ++vtCount;
        }
      else if (strncmp (line, "vn ", 3) == 0)
        {
          ++vnCount;
        }
      else if (strncmp (line, "f ", 2) == 0)
        {
          ++fCount;
        }
    }

  printf ("OBJ counts: v=%zu, vt=%zu, vn=%zu, f=%zu\n", vCount, vtCount,
          vnCount, fCount);

  if (vCount == 0 || fCount == 0)
    {
      fprintf (stderr, "OBJ enthaelt keine Vertices oder Faces\n");
      fclose (file);
      return NULL;
    }

  Vec3 *positions = malloc (vCount * sizeof (Vec3));
  Vec2 *texCoords = NULL;
  Vec3 *normals = NULL;

  if (vtCount > 0)
    {
      texCoords = malloc (vtCount * sizeof (Vec2));
    }

  if (vnCount > 0)
    {
      normals = malloc (vnCount * sizeof (Vec3));
    }

  if (!positions || (vtCount > 0 && !texCoords) || (vnCount > 0 && !normals))
    {
      fprintf (stderr, "malloc fehlgeschlagen\n");
      free (positions);
      free (texCoords);
      free (normals);
      fclose (file);
      return NULL;
    }

  // 8 floats pro Vertex: x y z, u v, nx ny nz
  // 3 Vertices pro Face
  float *output = malloc (fCount * 3 * 8 * sizeof (float));

  if (!output)
    {
      fprintf (stderr, "malloc output fehlgeschlagen\n");
      free (positions);
      free (texCoords);
      free (normals);
      fclose (file);
      return NULL;
    }

  rewind (file);

  size_t posIndex = 0;
  size_t texIndex = 0;
  size_t normIndex = 0;
  size_t outFloatIndex = 0;
  size_t realVertexCount = 0;

  // 2. Durchlauf: Daten lesen
  while (fgets (line, sizeof (line), file))
    {
      if (strncmp (line, "v ", 2) == 0)
        {
          if (sscanf (line, "v %f %f %f", &positions[posIndex][0],
                      &positions[posIndex][1], &positions[posIndex][2])
              == 3)
            {
              ++posIndex;
            }
        }
      else if (strncmp (line, "vt ", 3) == 0)
        {
          if (texCoords != NULL
              && sscanf (line, "vt %f %f", &texCoords[texIndex][0],
                         &texCoords[texIndex][1])
                     == 2)
            {
              ++texIndex;
            }
        }
      else if (strncmp (line, "vn ", 3) == 0)
        {
          if (normals != NULL
              && sscanf (line, "vn %f %f %f", &normals[normIndex][0],
                         &normals[normIndex][1], &normals[normIndex][2])
                     == 3)
            {
              ++normIndex;
            }
        }
      else if (strncmp (line, "f ", 2) == 0)
        {
          char t0[128];
          char t1[128];
          char t2[128];
          char extra[128];

          int parts
              = sscanf (line, "f %127s %127s %127s %127s", t0, t1, t2, extra);

          if (parts != 3)
            {
              fprintf (stderr, "Ueberspringe nicht-trianguliertes Face: %s",
                       line);
              continue;
            }

          FaceIndex idx[3];

          if (!parseFaceToken (t0, &idx[0]) || !parseFaceToken (t1, &idx[1])
              || !parseFaceToken (t2, &idx[2]))
            {
              fprintf (stderr, "Ueberspringe unlesbares Face: %s", line);
              continue;
            }

          for (int i = 0; i < 3; ++i)
            {
              if (!validIndex (idx[i].v, posIndex))
                {
                  fprintf (stderr, "Ungueltiger Positionsindex in Face: %s",
                           line);
                  goto skip_face;
                }

              if (idx[i].vt != 0 && !validIndex (idx[i].vt, texIndex))
                {
                  fprintf (stderr, "Ungueltiger Texturindex in Face: %s",
                           line);
                  goto skip_face;
                }

              if (idx[i].vn != 0 && !validIndex (idx[i].vn, normIndex))
                {
                  fprintf (stderr, "Ungueltiger Normalenindex in Face: %s",
                           line);
                  goto skip_face;
                }
            }

          for (int i = 0; i < 3; ++i)
            {
              Vec3 *p = &positions[idx[i].v - 1];

              output[outFloatIndex++] = (*p)[0];
              output[outFloatIndex++] = (*p)[1];
              output[outFloatIndex++] = (*p)[2];

              if (idx[i].vt != 0 && texCoords != NULL)
                {
                  Vec2 *t = &texCoords[idx[i].vt - 1];

                  output[outFloatIndex++] = (*t)[0];
                  output[outFloatIndex++] = (*t)[1];
                }
              else
                {
                  output[outFloatIndex++] = 0.0f;
                  output[outFloatIndex++] = 0.0f;
                }

              if (idx[i].vn != 0 && normals != NULL)
                {
                  Vec3 *n = &normals[idx[i].vn - 1];

                  output[outFloatIndex++] = (*n)[0];
                  output[outFloatIndex++] = (*n)[1];
                  output[outFloatIndex++] = (*n)[2];
                }
              else
                {
                  output[outFloatIndex++] = 0.0f;
                  output[outFloatIndex++] = 0.0f;
                  output[outFloatIndex++] = 0.0f;
                }

              ++realVertexCount;
            }

        skip_face:;
        }
    }

  fclose (file);

  free (positions);
  free (texCoords);
  free (normals);

  if (realVertexCount == 0)
    {
      fprintf (stderr, "Keine gueltigen Dreiecke geladen\n");
      free (output);
      return NULL;
    }

  *vertexCount = realVertexCount;

  printf ("OBJ geladen: %zu Vertices\n", realVertexCount);

  return output;
}
