// ConstParam.h
#ifndef CONSTPARAM
#define CONSTPARAM
#include "fftw3.h"

const float PI = 3.1415926535897932;
const float TWO_PI = 6.2831853071795864;

const int REAL = 0;
const int IMAG = 1;

const __int64 BufSize = 1048576;

float CosVal[5] = { 1,0.309016994374947,-0.809016994374947 ,-0.809016994374948,0.309016994374947 };
float SinVal[5] = { 0,0.951056516295154, 0.587785252292473, -0.587785252292473, -0.951056516295154 };

const float ACC = 40.0;
const float BIGNO = 1.0e10;
const float BIGNI = 1.0e-10;

#define max(a,b) (a>b?a:b)

typedef struct _Resampler_
{
	int _upRate;
	int _downRate;
	float* _transposedCoefs;
	float* _state;
	float* _stateEnd;
	int _paddedCoefCount;  // ceil(len(coefs)/upRate)*upRate
	int _coefsPerPhase;    // _paddedCoefCount / upRate
	__int64 _t; // "time" (modulo upRate)
	__int64 _xOffset;
}sResampler;
#endif

