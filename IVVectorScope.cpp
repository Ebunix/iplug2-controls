#include "IVVectorScope.h"

inline double gainToDb(double gain) {
  return 20.0 * log10(gain);
}

inline double dbToGain(double decibels) {
  return pow(10.0, decibels / 20.0);
}

void IVVectorScope::Draw(IGraphics& g)
{
  StereoRingBuffer srb = RingBufferSolidify(mBuffer);

  float cx = mRECT.MW();
  float cy = mRECT.MH();

  for (int i = 0; i < points; i++) {
    float left = 0, right = 0;
    for (int s = 0; s < step; s++) {
      int pos = i * step + s;
      left += srb.left[pos];
      right += srb.right[pos];
    }
    left = left * normalize;
    right = right * normalize;

    float x = mRECT.W() * 0.5f * left;
    float y = mRECT.H() * 0.5f * right;

    float xnew = cx + ( x * c - y * s );
    float ynew = cy + ( x * s + y * c );

    g.DrawCircle(color, xnew, ynew, 1);
    g.DrawLine(color, lastX, lastY, xnew, ynew, 0, 1.5f);

    lastX = xnew;
    lastY = ynew;
  }

  free(srb.left);
  free(srb.right);

  SetDirty(false);
}