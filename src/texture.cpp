#include "texture.h"
#include "CGL/color.h"

namespace CGL {

// Examines the enum parameters in sp and performs
// the appropriate sampling using the three helper functions below.
Color Texture::sample(const SampleParams &sp) {
  // Part 6: Fill in the functionality for sampling 
  //          nearest or bilinear in mipmap level 0, conditional on sp.psm

  // Part 7: Fill in full sampling (including trilinear), 
  //          conditional on sp.psm and sp.lsm
  float level = 0;
  if (sp.lsm == 1){
    level = max(Texture::get_level(sp), float(0));
  }
  if (sp.lsm == 2){
    level = max(Texture::get_level(sp), float(0));
    if (level == 0){
      return Texture::sample_bilinear(sp.uv, (int)level);
    }
    Color levelD_0 = Texture::sample_bilinear(sp.uv, (int) floor(level));
    Color levelD_1 = Texture::sample_bilinear(sp.uv, (int) ceil(level));

    float weight1 = level - floor(level);
    float weight0 = ceil(level) - level;

    float newR = weight0*levelD_0.r + weight1*levelD_1.r;
    float newG = weight0*levelD_0.g + weight1*levelD_1.g;
    float newB = weight0*levelD_0.b + weight1*levelD_1.b;
    float newA = weight0*levelD_0.a + weight1*levelD_1.a;

    return Color(newR, newG, newB, newA);
  }
  if (sp.psm == 0){
    return Texture::sample_nearest(sp.uv, (int)level);
  } else {
    return Texture::sample_bilinear(sp.uv, (int)level);
  }
}

// Given sp.du and sp.dv, returns the appropriate mipmap
// level to use for L_NEAREST or L_LINEAR filtering.
float Texture::get_level(const SampleParams &sp) {
  // Part 7: Fill this in.
  return log2(max(sqrt(pow(sp.du.x*width, 2) + pow((sp.dv.x*width), 2)), 
             sqrt(pow(sp.du.y*height, 2) + pow(sp.dv.y*height, 2))));
}

// Indexes into the level'th mipmap
// and returns the nearest pixel to (u,v)
Color Texture::sample_nearest(Vector2D uv, int level) {
  // Part 6: Fill this in.
  if (level > ((int)mipmap.size() - 1)){
    level = (int) (mipmap.size() - 1);
  }
  const MipLevel &myTex = mipmap[level];
  int texWidth = myTex.width;
  int texHeight = myTex.height;
  float denormX = uv.x*texWidth;
  float denormY = uv.y*texHeight;

  if ((int)(denormX + 0.5) < 0 || (int)(denormX + 0.5) >= texWidth) return Color(255.0, 255.0, 255.0, 255.0);
  if ((int)(denormY + 0.5) < 0 || (int)(denormY + 0.5) >= texHeight) return Color(255.0, 255.0, 255.0, 255.0);

  int nearestPixel = 4*((int)(denormX + 0.5) + ((int)(denormY + 0.5))*texWidth);
  unsigned char colorValues[4];
  colorValues[0] = myTex.texels[nearestPixel];
  colorValues[1] = myTex.texels[nearestPixel + 1];
  colorValues[2] = myTex.texels[nearestPixel + 2];
  colorValues[3] = myTex.texels[nearestPixel + 3];
  return Color(colorValues);
}

// Indexes into the level'th mipmap
// and returns a bilinearly weighted combination of
// the four pixels surrounding (u,v)
Color Texture::sample_bilinear(Vector2D uv, int level) {
  // Part 6: Fill this in.
  if (level > ((int)mipmap.size() - 1)){
    level = (int) (mipmap.size() - 1);
  }
  const MipLevel &myTex = mipmap[level];
  int texWidth = myTex.width;
  int texHeight = myTex.height;
  float denormX = uv.x*texWidth;
  float denormY = uv.y*texHeight;

  if ((int)(denormX + 0.5) < 0 || (int)(denormX + 0.5) >= texWidth) return Color(255.0, 255.0, 255.0, 255.0);
  if ((int)(denormY + 0.5) < 0 || (int)(denormY + 0.5) >= texHeight) return Color(255.0, 255.0, 255.0, 255.0);

  int blPixel = 4*((int)floor(denormX) + ((int)floor(denormY))*texWidth);
  int brPixel = 4*((int)ceil(denormX) + ((int)floor(denormY))*texWidth);
  int tlPixel = 4*((int)floor(denormX) + ((int)ceil(denormY))*texWidth);
  int trPixel = 4*((int)ceil(denormX) + ((int)ceil(denormY))*texWidth);
  unsigned char colorValues[4];
  float bottomTwoValues[4];
  float topTwoValues[4];
  float dx = denormX - floor(denormX);
  float dy = denormY - floor(denormY);
  bottomTwoValues[0] = myTex.texels[blPixel] + dx*(myTex.texels[brPixel] - myTex.texels[blPixel]);
  bottomTwoValues[1] = myTex.texels[blPixel + 1] + dx*(myTex.texels[brPixel + 1] - myTex.texels[blPixel + 1]);
  bottomTwoValues[2] = myTex.texels[blPixel + 2] + dx*(myTex.texels[brPixel + 2] - myTex.texels[blPixel + 2]);
  bottomTwoValues[3] = myTex.texels[blPixel + 3] + dx*(myTex.texels[brPixel + 3] - myTex.texels[blPixel + 3]);

  topTwoValues[0] = myTex.texels[tlPixel] + dx*(myTex.texels[trPixel] - myTex.texels[tlPixel]);
  topTwoValues[1] = myTex.texels[tlPixel + 1] + dx*(myTex.texels[trPixel + 1] - myTex.texels[tlPixel + 1]);
  topTwoValues[2] = myTex.texels[tlPixel + 2] + dx*(myTex.texels[trPixel + 2] - myTex.texels[tlPixel + 2]);
  topTwoValues[3] = myTex.texels[tlPixel + 3] + dx*(myTex.texels[trPixel + 3] - myTex.texels[tlPixel + 3]);

  colorValues[0] = (char) (bottomTwoValues[0] + dy*(topTwoValues[0] - bottomTwoValues[0]));
  colorValues[1] = (char) (bottomTwoValues[1] + dy*(topTwoValues[1] - bottomTwoValues[1]));
  colorValues[2] = (char) (bottomTwoValues[2] + dy*(topTwoValues[2] - bottomTwoValues[2]));
  colorValues[3] = (char) (bottomTwoValues[3] + dy*(topTwoValues[3] - bottomTwoValues[3]));
  return Color(colorValues);
}



/****************************************************************************/



inline void uint8_to_float(float dst[4], unsigned char *src) {
  uint8_t *src_uint8 = (uint8_t *)src;
  dst[0] = src_uint8[0] / 255.f;
  dst[1] = src_uint8[1] / 255.f;
  dst[2] = src_uint8[2] / 255.f;
  dst[3] = src_uint8[3] / 255.f;
}

inline void float_to_uint8(unsigned char *dst, float src[4]) {
  uint8_t *dst_uint8 = (uint8_t *)dst;
  dst_uint8[0] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[0])));
  dst_uint8[1] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[1])));
  dst_uint8[2] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[2])));
  dst_uint8[3] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[3])));
}

void Texture::generate_mips(int startLevel) {

  // make sure there's a valid texture
  if (startLevel >= mipmap.size()) {
    std::cerr << "Invalid start level";
  }

  // allocate sublevels
  int baseWidth = mipmap[startLevel].width;
  int baseHeight = mipmap[startLevel].height;
  int numSubLevels = (int)(log2f((float)max(baseWidth, baseHeight)));

  numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
  mipmap.resize(startLevel + numSubLevels + 1);

  int width = baseWidth;
  int height = baseHeight;
  for (int i = 1; i <= numSubLevels; i++) {

    MipLevel &level = mipmap[startLevel + i];

    // handle odd size texture by rounding down
    width = max(1, width / 2);
    //assert (width > 0);
    height = max(1, height / 2);
    //assert (height > 0);

    level.width = width;
    level.height = height;
    level.texels = vector<unsigned char>(4 * width * height);
  }

  // create mips
  int subLevels = numSubLevels - (startLevel + 1);
  for (int mipLevel = startLevel + 1; mipLevel < startLevel + subLevels + 1;
       mipLevel++) {

    MipLevel &prevLevel = mipmap[mipLevel - 1];
    MipLevel &currLevel = mipmap[mipLevel];

    int prevLevelPitch = prevLevel.width * 4; // 32 bit RGBA
    int currLevelPitch = currLevel.width * 4; // 32 bit RGBA

    unsigned char *prevLevelMem;
    unsigned char *currLevelMem;

    currLevelMem = (unsigned char *)&currLevel.texels[0];
    prevLevelMem = (unsigned char *)&prevLevel.texels[0];

    float wDecimal, wNorm, wWeight[3];
    int wSupport;
    float hDecimal, hNorm, hWeight[3];
    int hSupport;

    float result[4];
    float input[4];

    // conditional differentiates no rounding case from round down case
    if (prevLevel.width & 1) {
      wSupport = 3;
      wDecimal = 1.0f / (float)currLevel.width;
    } else {
      wSupport = 2;
      wDecimal = 0.0f;
    }

    // conditional differentiates no rounding case from round down case
    if (prevLevel.height & 1) {
      hSupport = 3;
      hDecimal = 1.0f / (float)currLevel.height;
    } else {
      hSupport = 2;
      hDecimal = 0.0f;
    }

    wNorm = 1.0f / (2.0f + wDecimal);
    hNorm = 1.0f / (2.0f + hDecimal);

    // case 1: reduction only in horizontal size (vertical size is 1)
    if (currLevel.height == prevLevel.height) {
      //assert (currLevel.height == 1);

      for (int i = 0; i < currLevel.width; i++) {
        wWeight[0] = wNorm * (1.0f - wDecimal * i);
        wWeight[1] = wNorm * 1.0f;
        wWeight[2] = wNorm * wDecimal * (i + 1);

        result[0] = result[1] = result[2] = result[3] = 0.0f;

        for (int ii = 0; ii < wSupport; ii++) {
          uint8_to_float(input, prevLevelMem + 4 * (2 * i + ii));
          result[0] += wWeight[ii] * input[0];
          result[1] += wWeight[ii] * input[1];
          result[2] += wWeight[ii] * input[2];
          result[3] += wWeight[ii] * input[3];
        }

        // convert back to format of the texture
        float_to_uint8(currLevelMem + (4 * i), result);
      }

      // case 2: reduction only in vertical size (horizontal size is 1)
    } else if (currLevel.width == prevLevel.width) {
      //assert (currLevel.width == 1);

      for (int j = 0; j < currLevel.height; j++) {
        hWeight[0] = hNorm * (1.0f - hDecimal * j);
        hWeight[1] = hNorm;
        hWeight[2] = hNorm * hDecimal * (j + 1);

        result[0] = result[1] = result[2] = result[3] = 0.0f;
        for (int jj = 0; jj < hSupport; jj++) {
          uint8_to_float(input, prevLevelMem + prevLevelPitch * (2 * j + jj));
          result[0] += hWeight[jj] * input[0];
          result[1] += hWeight[jj] * input[1];
          result[2] += hWeight[jj] * input[2];
          result[3] += hWeight[jj] * input[3];
        }

        // convert back to format of the texture
        float_to_uint8(currLevelMem + (currLevelPitch * j), result);
      }

      // case 3: reduction in both horizontal and vertical size
    } else {

      for (int j = 0; j < currLevel.height; j++) {
        hWeight[0] = hNorm * (1.0f - hDecimal * j);
        hWeight[1] = hNorm;
        hWeight[2] = hNorm * hDecimal * (j + 1);

        for (int i = 0; i < currLevel.width; i++) {
          wWeight[0] = wNorm * (1.0f - wDecimal * i);
          wWeight[1] = wNorm * 1.0f;
          wWeight[2] = wNorm * wDecimal * (i + 1);

          result[0] = result[1] = result[2] = result[3] = 0.0f;

          // convolve source image with a trapezoidal filter.
          // in the case of no rounding this is just a box filter of width 2.
          // in the general case, the support region is 3x3.
          for (int jj = 0; jj < hSupport; jj++)
            for (int ii = 0; ii < wSupport; ii++) {
              float weight = hWeight[jj] * wWeight[ii];
              uint8_to_float(input, prevLevelMem +
                                        prevLevelPitch * (2 * j + jj) +
                                        4 * (2 * i + ii));
              result[0] += weight * input[0];
              result[1] += weight * input[1];
              result[2] += weight * input[2];
              result[3] += weight * input[3];
            }

          // convert back to format of the texture
          float_to_uint8(currLevelMem + currLevelPitch * j + 4 * i, result);
        }
      }
    }
  }
}

}