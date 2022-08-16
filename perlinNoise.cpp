#include"perlinNoise.h"
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void perlinNoise::perLinNoise(int nCount, float* fSeed, float fScalingBais, int nOctaves, float* fOutPut)
{
	for (int x = 0; x < nCount; ++x)
	{
		float fNoise = 0.0f;
		float fScale = 1.0f;
		float fScaleAcc = 0.0f;

		for (int o = 0; o < nOctaves; ++o)
		{
			int nPitch = nCount >> o;	//next octave
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;	//finding two points in the array
			float fBlend = (float)(x - nSample1) / (float)nPitch;	//distance from x to sample1 relative to the size of the pitch
			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];	//linear interpolation
			fNoise += fSample * fScale;	//suming the noise from eath octave
			fScaleAcc += fScale;
			fScale = fScale / fScalingBais;

		}
		fOutPut[x] = fNoise / fScaleAcc;	//divide by the sum of the scales to get num between 0 to 1
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void perlinNoise::perLinNoise2D(int nWidth, int nHeight, float* fSeed, float fScalingBais, int nOctaves, float* fOutPut)
{
	for (int x = 0; x < nWidth; ++x)
		for (int y = 0; y < nHeight; ++y)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleAcc = 0.0f;

			for (int o = 0; o < nOctaves; ++o)
			{
				int nPitch = nWidth >> o;	//next octave
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;	//finding two points in the array

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;	//distance from x to sample1 X relative to the size of the pitch
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;	//distance from y to sample1 Y relative to the size of the pitch

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY1 * nWidth + nSampleX2];	//linear interpolation between the X values
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nWidth + nSampleX1] + fBlendX * fSeed[nSampleY2 * nWidth + nSampleX2];	//linear interpolation between the Y values

				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScaleAcc += fScale;
				fScale = fScale / fScalingBais;

			}
			fOutPut[y * nWidth + x] = fNoise / fScaleAcc;	//divide by the sum of the scales to get num between 0 to 1
		}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------