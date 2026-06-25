#version 330 core

in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float time;

out vec4 FragColor;

// random number generation
float rand(vec2 p)
{
  return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

// basic noise pattern
float noise(vec2 p)
{
  vec2 i = floor(p);
  vec2 f = fract(p);

  float a = rand(i);
  float b = rand(i + vec2(1.0, 0.0));
  float c = rand(i + vec2(0.0, 1.0));
  float d = rand(i + vec2(1.0, 1.0));

  vec2 u = f * f * (3.0 - 2.0 * f);

  return mix(a, b, u.x) +
    (c - a) * u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

void main()
{
  vec2 uv = TexCoord;

  // Camera shake
  float shake =
    sin(time * 85.0) *
    sin(time * 113.0) *
    0.0015;
  uv.x += shake;

  // CRT tube distortion
  vec2 crt = uv * 2.0 - 1.0;
  float dist = dot(crt, crt);
  crt *= 1.0 + dist * 0.08;
  uv = crt * 0.5 + 0.5;

  // clip bounds
  if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
  {
    FragColor = vec4(0.0);
    return;
  }

  // chromatic abberation
  float aberration = 0.002 + dist * 0.004;

  float r = texture(screenTexture, uv + vec2(aberration, 0.0)).r;
  float g = texture(screenTexture, uv).g;
  float b = texture(screenTexture, uv - vec2(aberration, 0.0)).b;

  vec3 color = vec3(r, g, b);

  // grayscale
  // float gray = dot(color, vec3(0.299, 0.587, 0.114));
  // color = vec3(gray);

  // green tint
  color *= vec3(0.80, 1.0, 0.85);

  // static
  float staticNoise = rand(uv * vec2(1920.0, 1080.0) + time * 100.0);
  color += (staticNoise - 0.5) * 0.18;

  // grain
  float grain = noise(uv * 900.0 + time * 20.0);
  color += (grain - 0.5) * 0.05;

  // scanlines
  float scanlines = sin(uv.y * 1200.0);
  color -= scanlines * 0.04;

  // moving bars
  float bar1Y = fract(time * 0.15);
  float bar1 = smoothstep(0.06, 0.0, abs(uv.y - bar1Y));
  color += bar1 * 0.25;

  float bar2Y = fract(time * 0.23 + 0.4);

  float bar2 = smoothstep(0.04, 0.0, abs(uv.y - bar2Y));
  color -= bar2 * 0.15;

  // flicker
  float flicker = 0.95 + 0.04 * sin(time * 55.0) + 0.02 * sin(time * 127.0);
  color *= flicker;

  // white bursts
  float burst = step(0.985, rand(vec2(floor(time * 5.0), 0.0)));
  color += burst * 0.45;

  // signal loss
  float stripe = step(0.97, rand(vec2(floor(uv.y * 80.0), floor(time * 8.0))));

  color -= stripe * 0.25;

  // crt edge simulation
  float edge =
    smoothstep(0.0, 0.25, uv.x) *
    smoothstep(0.0, 0.25, uv.y) *
    smoothstep(0.0, 0.25, 1.0 - uv.x) *
    smoothstep(0.0, 0.25, 1.0 - uv.y);

  color *= edge + 0.15;

  // vignette
  float vignette = 1.0 - dist * 0.65;
  color *= vignette;

  // clamp
  color = clamp(color, 0.0, 1.0);

  FragColor = vec4(color, 1.0);
}
