#ifndef __MAIN_H_
#define __MAIN_H_
#include <assert.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include "libs/bitmap.h"
#include "mandelColours.h"
#include "mandelCompute.h"


struct colourGradientStep {
  double step;
  pixel colour;
};

#define colourGradientSteps 7

// first step MUST be 0.0
// last step MUST be 1.0
static const struct colourGradientStep colourGradient[colourGradientSteps] = {
	{ .step = 0.0 , { .r = 0  , .g = 0  , .b = 0   }},
	{ .step = 0.03, { .r = 0  , .g = 7  , .b = 100 }},
	{ .step = 0.16, { .r = 32 , .g = 107, .b = 203 }},
	{ .step = 0.42, { .r = 237, .g = 255, .b = 255 }},
	{ .step = 0.64, { .r = 255, .g = 170, .b = 0   }},
	{ .step = 0.86, { .r = 0  , .g = 2  , .b = 0   }},
	{ .step = 1.0,  { .r = 0  , .g = 0  , .b = 0   }}
};

static pixel *colourMap = NULL;
static pixel const borderFillColour = { .r = 255, .g = 255, .b = 255 };
static pixel const borderComputeColour = { .r = 255, .g = 0, .b = 0 };


int initColourMap(unsigned int const colourMapSize)  {
  assert(colourGradient[0].step == 0.0);
  assert(colourGradient[colourGradientSteps - 1].step == 1.0);
	colourMap = malloc(colourMapSize * sizeof(pixel));
  if (colourMap == NULL) {
    return 1;
  }
  pixel colour = { .r = 0, .g = 0, .b = 0 };
  double pos = 0.0;
  unsigned int ins = 0;

  for (unsigned int i = 0; i < colourGradientSteps && ins < colourMapSize; i++) {
    assert(pos <= colourGradient[i].step);
    int r = (int) colourGradient[i].colour.r - colour.r;
    int g = (int) colourGradient[i].colour.g - colour.g;
    int b = (int) colourGradient[i].colour.b - colour.b;
    unsigned int const max = ceil(((double) colourMapSize * (colourGradient[i].step - pos)));
	for (unsigned int i = 0; i < max && ins < colourMapSize; i++) {
      double blend = (double) i / max;
      colourMap[ins].r = colour.r + (blend * r);
      colourMap[ins].g = colour.g + (blend * g);
      colourMap[ins].b = colour.b + (blend * b);
      ins++;
    }
    pos = colourGradient[i].step;
    colour = colourGradient[i].colour;
  }
  return 0;
}

void freeColourMap() {
  if (colourMap)
    free(colourMap);
}

pixel const *getDwellColour(unsigned int const y, unsigned int const x, dwellType const dwell) {
	static const double log2 = 0.693147180559945309417232121458176568075500134360255254120;
	switch (dwell) {
		case dwellBorderFill:
			return &borderFillColour;
			break;
		case dwellBorderCompute:
			return &borderComputeColour;
			break;
	}
	double complex z = x + y * I;
	unsigned int index = dwell + 1 - log(log(sqrt(creal(z) * creal(z) + cimag(z) * cimag(z))/log2));
	return &colourMap[index % maxDwell];
}

#endif // __MAIN_H_
