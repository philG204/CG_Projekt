#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>


int
inverse(GLfloat out[9], const GLfloat in[9])
{
    GLfloat mHash[9];

    const GLfloat det =
        (in[0] * in[4] * in[8]) +
        (in[3] * in[7] * in[2]) +
        (in[6] * in[1] * in[5]) -
        (in[6] * in[4] * in[2]) -
        (in[0] * in[7] * in[5]) -
        (in[3] * in[1] * in[8]);

    if (fabsf(det) < 0.000001f)
    {
        printf("inverse: Matrix ist nicht invertierbar, det = %f\n", det);

        for (int i = 0; i < 9; i++)
        {
            out[i] = 0.0f;
        }

        return 0;
    }

    mHash[0] =  (in[4] * in[8]) - (in[5] * in[7]);
    mHash[1] = -((in[1] * in[8]) - (in[2] * in[7]));
    mHash[2] =  (in[1] * in[5]) - (in[2] * in[4]);

    mHash[3] = -((in[3] * in[8]) - (in[5] * in[6]));
    mHash[4] =  (in[0] * in[8]) - (in[2] * in[6]);
    mHash[5] = -((in[0] * in[5]) - (in[2] * in[3]));

    mHash[6] =  (in[3] * in[7]) - (in[4] * in[6]);
    mHash[7] = -((in[0] * in[7]) - (in[1] * in[6]));
    mHash[8] =  (in[0] * in[4]) - (in[1] * in[3]);

    const GLfloat invDet = 1.0f / det;

    for (int i = 0; i < 9; i++)
    {
        out[i] = mHash[i] * invDet;
    }

    return 1;
}

void
normalize (GLfloat *v)
{
  assert (v != NULL);

  GLfloat len = sqrtf (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  if (len > 0.0f)
    {
      v[0] /= len;
      v[1] /= len;
      v[2] /= len;
    }
}

void
cross (const GLfloat *a, const GLfloat *b, GLfloat *out)
{
  assert (a != NULL);
  assert (b != NULL);
  assert (out != NULL);

  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
}

GLfloat
dot (const GLfloat *a, const GLfloat *b)
{
  assert (a != NULL);
  assert (b != NULL);

  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
multiplyMatrices (GLfloat *out, const GLfloat *a, GLfloat *b)
{
  assert (a != NULL);
  assert (b != NULL);
  assert (out != NULL);

  GLfloat temp[16];

  for (int col = 0; col < 4; ++col)
  {
    for (int row = 0; row < 4; ++row)
    {
      float sum = 0.0f;
      for (int k = 0; k < 4; ++k)
      {
        sum += a[k * 4 + row] * b[col * 4 + k];
      }
      temp[col * 4 + row] = sum;
    }
  }

  for (int i = 0; i < 16; ++i)
  {
    out[i] = temp[i];
  }
}

void
identity (GLfloat *out)
{
  assert (out != NULL);

  for (int i = 0; i < 16; i++)
    {
      out[i] = 0.0f;
    }

  out[0] = 1.0f;
  out[5] = 1.0f;
  out[10] = 1.0f;
  out[15] = 1.0f;
}

void
translate (GLfloat *out, const GLfloat *in, GLfloat *v)
{
  assert (out != NULL);
  assert (in != NULL);
  assert (v != NULL);

  GLfloat t[16];

  identity (t);
  t[12] = v[0];
  t[13] = v[1];
  t[14] = v[2];

  multiplyMatrices (out, in, t);
}

void
rotatex (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  assert (out != NULL);
  assert (in != NULL);

  GLfloat r[16];

  GLfloat c = cosf (angle);
  GLfloat s = sinf (angle);

  identity (r);

  // 2.Spalte:
  r[4] = in[4] * c + in[8] * s;
  r[5] = in[5] * c + in[9] * s;
  r[6] = in[6] * c + in[10] * s;
  r[7] = in[7] * c + in[11] * s;

  // 3.Spalte:
  r[8] = in[4] * (-s) + in[8] * c;
  r[9] = in[5] * (-s) + in[9] * c;
  r[10] = in[6] * (-s) + in[10] * c;
  r[11] = in[7] * (-s) + in[11] * c;

  multiplyMatrices (out, in, r);
}

void
rotatey (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  assert (out != NULL);
  assert (in != NULL);

  GLfloat r[16];

  GLfloat c = cosf (angle);
  GLfloat s = sinf (angle);

  identity (r);
  r[0] = c;
  r[2] = -s;
  r[8] = s;
  r[10] = c;

  multiplyMatrices (out, in, r);
}

void
rotatez (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  assert (out != NULL);
  assert (in != NULL);

  GLfloat r[16];

  GLfloat c = cosf (angle);
  GLfloat s = sinf (angle);

  identity (r);
  r[0] = c;
  r[4] = -s;
  r[1] = s;
  r[5] = c;

  multiplyMatrices (out, in, r);
}

void
scale (GLfloat *out, GLfloat *in, const GLfloat *v)
{
  assert (out != NULL);
  assert (in != NULL);
  assert (v != NULL);

  GLfloat s[16];

  identity (s);
  s[0] = v[0];
  s[5] = v[1];
  s[10] = v[2];

  multiplyMatrices (out, in, s);
}

void
lookAt (GLfloat *out, const GLfloat *eye, const GLfloat *center,
        const GLfloat *up)
{
  assert (out != NULL);
  assert (eye != NULL);
  assert (center != NULL);
  assert (up != NULL);

  GLfloat f[3]
      = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
  normalize (f);

  GLfloat s[3];
  cross (f, up, s);
  normalize (s);

  GLfloat u[3];
  cross (s, f, u);

  out[0] = s[0];
  out[4] = s[1];
  out[8] = s[2];
  out[12] = -dot (s, eye);
  out[1] = u[0];
  out[5] = u[1];
  out[9] = u[2];
  out[13] = -dot (u, eye);
  out[2] = -f[0];
  out[6] = -f[1];
  out[10] = -f[2];
  out[14] = dot (f, eye);
  out[3] = 0.0f;
  out[7] = 0.0f;
  out[11] = 0.0f;
  out[15] = 1.0f;
}

void
perspective (GLfloat *out, float fovy, float aspect, float near, float far)
{
  assert (out != NULL);

  for (int i = 0; i < 16; i++)
    {
      out[i] = 0.0f;
    }

  GLfloat tanHalfFovy = tanf (fovy / 2.0f);

  out[0] = 1.0f / (aspect * tanHalfFovy);
  out[5] = 1.0f / tanHalfFovy;
  out[10] = -(far + near) / (far - near);
  out[11] = -1.0f;
  out[14] = -(2.0f * far * near) / (far - near);
}
