#ifndef __MANDELCOMPUTE_H_
#define __MANDELCOMPUTE_H_
#include <complex.h>
#include <math.h>

typedef int dwellType;
#define dwellUncomputed (dwellType) (-1)
#define dwellBorderFill (dwellType) (-2)
#define dwellBorderCompute (dwellType) (-3)

dwellType maxDwell;
double complex dc;
double complex cmin;
unsigned int resolution;


dwellType pixelDwell(unsigned int const y, unsigned int const x) {
	double complex const fdc = (((double) x / resolution) * creal(dc)) + (((double) y / resolution) * cimag(dc) * I);
	double complex const c = cmin + fdc;
	double complex z = c;
	dwellType dwell = 0;

	// Exit condition: dwell is maxDwell or |z| >= 4
	while(dwell < maxDwell && sqrt(creal(z) * creal(z) + cimag(z) * cimag(z)) < 4) {
		// z = z² + c
		z = (z * z) + c;
		dwell++;
	}

	return dwell;
}

dwellType commonBorder(dwellType *buffer,
					   unsigned int const atY,
					   unsigned int const atX,
					   unsigned int const blockSize)
{
	unsigned int const yMax = (resolution > atY + blockSize - 1) ? atY + blockSize - 1 : resolution - 1;
	unsigned int const xMax = (resolution > atX + blockSize - 1) ? atX + blockSize - 1 : resolution - 1;
	dwellType commonDwell = dwellUncomputed;
	for (unsigned int i = 0; i < blockSize; i++) {
		for (unsigned int s = 0; s < 4; s++) {
			unsigned const int y = s % 2 == 0 ? atY + i : (s == 1 ? yMax : atY);
			unsigned const int x = s % 2 != 0 ? atX + i : (s == 0 ? xMax : atX);
			if (y < resolution && x < resolution) {
				dwellType dwell = buffer[y * resolution + x];
				if (dwell == dwellUncomputed) {
					dwell = pixelDwell(y, x);
					buffer[y * resolution + x] = dwell;
				}
				if (commonDwell == -1) {
					commonDwell = dwell;
				} else if (commonDwell != dwell) {
					return dwellUncomputed;
				}
			}
		}
	}
	return commonDwell;
}


void computeBlock(dwellType *buffer,
				  unsigned int const atY,
				  unsigned int const atX,
				  unsigned int const blockSize)
{
	unsigned int const yMax = (resolution > atY + blockSize) ? atY + blockSize : resolution;
	unsigned int const xMax = (resolution > atX + blockSize) ? atX + blockSize : resolution;
	for (unsigned int y = atY; y < yMax; y++) {
		for (unsigned int x = atX; x < xMax; x++) {
			if (buffer[y * resolution +x] == dwellUncomputed) {
				buffer[y * resolution + x] = pixelDwell(y, x);
			}
 		}
	}
}

void fillBlock(dwellType *buffer,
			   dwellType const dwell,
			   unsigned int const atY,
			   unsigned int const atX,
			   unsigned int const blockSize)
{
	unsigned int const yMax = (resolution > atY + blockSize) ? atY + blockSize : resolution;
	unsigned int const xMax = (resolution > atX + blockSize) ? atX + blockSize : resolution;
	for (unsigned int y = atY; y < yMax; y++) {
		for (unsigned int x = atX; x < xMax; x++) {
			buffer[y * resolution + x] = dwell;
		}
	}
}

void markBorder(dwellType *buffer,
				dwellType const dwell,
				unsigned int const atY,
				unsigned int const atX,
				unsigned int const blockSize)
{
	unsigned int const yMax = (resolution > atY + blockSize - 1) ? atY + blockSize - 1 : resolution - 1;
	unsigned int const xMax = (resolution > atX + blockSize - 1) ? atX + blockSize - 1 : resolution - 1;
	for (unsigned int i = 0; i < blockSize; i++) {
		for (unsigned int s = 0; s < 4; s++) {
			unsigned const int y = s % 2 == 0 ? atY + i : (s == 1 ? yMax : atY);
			unsigned const int x = s % 2 != 0 ? atX + i : (s == 0 ? xMax : atX);
			if (y < resolution && x < resolution) {
				buffer[y * resolution + x] = dwell;
			}
		}
	}
}


#endif // __MANDELCOMPUTE_H_
