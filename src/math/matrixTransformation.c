#include <assert.h>
#include <math.h>

#include <GL/glew.h>

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
cross (GLfloat *out, const GLfloat *a, const GLfloat *b)
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

  // in the special case where the input and output are the same variable
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

  identity(r);

  r[5] = c;
  r[6] = s;
  r[9] = -s;
  r[10] = c;

  multiplyMatrices(out, in, r);
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
  cross (s, f, up);
  normalize (s);

  GLfloat u[3];
  cross (u, s, f);

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
