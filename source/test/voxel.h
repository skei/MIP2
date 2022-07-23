#ifndef voxel_included
#define voxel_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_rgba.h"

/*
  - hardcoded 320*240
  - realtime light calc?
  - x/z angle
  - height
*/

//----------------------------------------------------------------------

typedef struct mip_voxel_t {
  uint32_t* DBuf;
  uint8_t   HMap[256*256];
  uint8_t   CMap[256*256];
  float   FOV;
  int     LastY[320];
  int     LastC[320];
} mip_voxel_t;

//----------------------------------------------------------------------

mip_voxel_t* voxel_create(void) {
  mip_voxel_t* voxel = (mip_voxel_t*)malloc( sizeof(mip_voxel_t) );
  memset((uint8_t*)voxel,0,sizeof(mip_voxel_t));
  voxel->FOV = MIP_PI / 3.5;
  return voxel;
}

//----------

void voxel_destroy(mip_voxel_t* voxel) {
  if (!voxel) return;
  free(voxel);
  voxel = nullptr;
}

//----------------------------------------------------------------------

int voxel_clamp(int x) {
  return (x < 0 ? 0 : (x > 255 ? 255 : x));
}

//----------

void voxel_make_map(mip_voxel_t* voxel) {
  int p, p2, k, k2, i, j;
  int a, b, c , d;
  voxel->HMap[0] = 128;
  for (p = 256; p > 1; p=p2) {
    p2 = p >> 1;
    k = p * 8 + 20;
    k2 = k >> 1;
    for (i = 0; i < 256; i += p) {
      for (j = 0; j < 256;  j+= p) {
        a = voxel->HMap[(i << 8) + j];
        b = voxel->HMap[(((i + p) & 255) << 8) + j];
        c = voxel->HMap[(i << 8) + ((j + p) & 255)];
        d = voxel->HMap[(((i + p) & 255) << 8) + ((j + p) & 255)];
        voxel->HMap[(i << 8) + ((j + p2) & 255)] = voxel_clamp(((a + c) >> 1) + (rand() % k - k2));
        voxel->HMap[(((i + p2) & 255) << 8) + ((j + p2) & 255)] = voxel_clamp(((a + b + c + d) >> 2) + (rand() % k - k2));
        voxel->HMap[(((i + p2) & 255) << 8) + j] = voxel_clamp(((a + b) >> 1) + (rand() % k - k2));
      }
    }
  }
  for (k = 0; k < 3; k++) {
    for (i = 0; i < 256 * 256; i += 256) {
      for (j = 0; j < 256; j++) {
        voxel->HMap[i + j] = (voxel->HMap[((i + 256) & 0xFF00) + j] + voxel->HMap[i + ((j + 1) & 0xFF)]
        + voxel->HMap[((i - 256) & 0xFF00) + j] + voxel->HMap[i + ((j - 1) & 0xFF)]) >> 2;
      }
    }
  }
  for (i = 0; i < 256 * 256; i += 256) {
    for (j = 0; j < 256; j++) {
      k = 128 + (voxel->HMap[((i + 256) & 0xFF00) + ((j + 1) & 255)] - voxel->HMap[i + j]) * 6;
      if (k < 0) k = 0;
      if (k > 255) k = 255;
      voxel->CMap[i + j] = k;
    }
  }
}

//----------

void voxel_line(mip_voxel_t* voxel, int x0, int y0, int x1, int y1, int hy, int s) {
  int sx = (x1 - x0) / 320;
  int sy = (y1 - y0) / 320;
  for (int i = 0; i < 320; i++) {
    int u0 = (x0 >> 16) & 0xFF;
    int a = (x0 >> 8) & 255;
    int v0 = ((y0 >> 8) & 0xFF00);
    int b = (y0 >> 8) & 255;
    int u1 = (u0 + 1) & 0xFF;
    int v1 = (v0 + 256) & 0xFF00;
    int h0 = voxel->HMap[u0 + v0];
    int h2 = voxel->HMap[u0 + v1];
    int h1 = voxel->HMap[u1 + v0];
    int h3 = voxel->HMap[u1 + v1];
    h0 = (h0 << 8) + a * (h1 - h0);
    h2 = (h2 << 8) + a * (h3 - h2);
    int h = ((h0 << 8) + b * (h2 - h0)) >> 16;
    h0 = voxel->CMap[u0 + v0];
    h2 = voxel->CMap[u0 + v1];
    h1 = voxel->CMap[u1 + v0];
    h3 = voxel->CMap[u1 + v1];
    h0 = (h0 << 8) + a * (h1 - h0);
    h2 = (h2 << 8) + a * (h3 - h2);
    int c = ((h0 << 8) + b * (h2 - h0));
    int y = (((h - hy) * s) >> 11) + (240 / 2);
    if (y < (a = voxel->LastY[i])) {
      //unsigned char *b = DBuf + (a << 8) + (a << 6) + i;
      uint32_t* b = voxel->DBuf + (a << 8) + (a << 6) + i;
      int sc,cc;
      if (voxel->LastC[i] == -1) voxel->LastC[i] = c;
      sc = (c - voxel->LastC[i]) / (a - y);
      cc = voxel->LastC[i];
      if (a > 240 - 1) {
        b -= (a - (240 - 1)) * 320;
        cc += (a - (240 - 1)) * sc;
        a = 240 - 1;
      }
      if (y < 0) y = 0;
      while (y < a) {

        uint8_t clr = cc >> 16;
        *b = MIP_RGBA(clr,clr,clr,255);

        cc += sc;
        b -= 320;
        a--;
      }
      voxel->LastY[i] = y;
    }
    voxel->LastC[i] = c;
    x0 += sx;
    y0 += sy;
  }
}

//----------------------------------------------------------------------

void voxel_render_frame(mip_voxel_t* voxel, uint32_t* buffer, int x0, int y0, float aa) {
  //Video->Clear();
  voxel->DBuf = buffer;
  int u0 = (x0 >> 16) & 0xFF;
  int a = (x0 >> 8) & 255;
  int v0 = ((y0 >> 8) & 0xFF00);
  int b = (y0 >> 8) & 255;
  int u1 = (u0 + 1) & 0xFF;
  int v1 = (v0 + 256) & 0xFF00;
  int h0 = voxel->HMap[u0 + v0];
  int h2 = voxel->HMap[u0 + v1];
  int h1 = voxel->HMap[u1 + v0];
  int h3 = voxel->HMap[u1 + v1];
  h0 = (h0 << 8) + a * (h1 - h0);
  h2 = (h2 << 8) + a * (h3 - h2);
  int h = ((h0 << 8) + b * (h2 - h0)) >> 16;
  //int h = 0;
  int d;
  for (d=0; d<320; d++) voxel->LastY[d] = 240;
  memset( (uint8_t*)voxel->LastC, -1, 320 * sizeof(int));
  for (d=0; d<120; d += (1+(d>>6)) ) {
    voxel_line(
      voxel,
      x0 + d * 65536 * cosf(aa - voxel->FOV),
      y0 + d * 65536 * sinf(aa - voxel->FOV),
      x0 + d * 65536 * cosf(aa + voxel->FOV),
      y0 + d * 65536 * sinf(aa + voxel->FOV),
      h - 30,
      120 * 256 / (d+1)
    );
  }
}

//----------------------------------------------------------------------
#endif
