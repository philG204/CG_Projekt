#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void
normalize (GLfloat *v)
{
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
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
}

GLfloat
dot (const GLfloat *a, const GLfloat *b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void
multiplyMatrices (GLfloat *out, const GLfloat *A, GLfloat *B)
{
  GLfloat temp[16];

  for (int col = 0; col < 4; ++col)
    {
      for (int row = 0; row < 4; ++row)
        {
          float sum = 0.0f;
          for (int k = 0; k < 4; ++k)
            {
              sum += A[k * 4 + row] * B[col * 4 + k];
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
  GLfloat T[16];

  identity (T);
  T[12] = v[0];
  T[13] = v[1];
  T[14] = v[2];

  multiplyMatrices (out, in, T);
}

void
rotatez (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  GLfloat R[16];

  GLfloat c = cosf (angle);
  GLfloat s = sinf (angle);

  identity (R);
  R[0] = c;
  R[4] = -s;
  R[1] = s;
  R[5] = c;

  multiplyMatrices (out, in, R);
}

void
rotatey (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  GLfloat R[16];

  GLfloat c = cosf (angle);
  GLfloat s = sinf (angle);

  identity (R);
  R[0] = c;
  R[2] = -s;
  R[8] = s;
  R[10] = c;

  multiplyMatrices (out, in, R);
}

void
rotatex (GLfloat *out, const GLfloat *in, GLfloat angle)
{
  GLfloat c = cosf (angle), s = sinf (angle), R[16];

  identity (R);

  // 2.Spalte:
  R[4] = in[4] * c + in[8] * s;
  R[5] = in[5] * c + in[9] * s;
  R[6] = in[6] * c + in[10] * s;
  R[7] = in[7] * c + in[11] * s;

  // 3.Spalte:
  R[8] = in[4] * (-s) + in[8] * c;
  R[9] = in[5] * (-s) + in[9] * c;
  R[10] = in[6] * (-s) + in[10] * c;
  R[11] = in[7] * (-s) + in[11] * c;

  multiplyMatrices (out, in, R);
}

void
scale (GLfloat *out, GLfloat *in, const GLfloat *v)
{
  GLfloat S[16];

  identity (S);
  S[0] = v[0];
  S[5] = v[1];
  S[10] = v[2];

  multiplyMatrices (out, in, S);
}

void
lookAt (GLfloat *out, const GLfloat *eye, const GLfloat *center,
        const GLfloat *up)
{
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
