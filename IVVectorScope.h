#pragma once
#include "IControl.h"
#include "StereoRingBuffer.h"

using namespace iplug::igraphics;

class IVVectorScope : public IControl
{
public:
  IColor color = IColor(30, 255, 255, 255);

  IVVectorScope(const IRECT& bounds, StereoRingBuffer* buffer) : IControl(bounds) {
    mBuffer = buffer;
    s = sin(0.785398 * 5); // 225 deg in radians
    c = cos(0.785398 * 5);
    SetPointCount(mBuffer->used);
  }

  void Draw(IGraphics& g) override;
  inline void SetPointCount(int points) {
    this->points = points;
    step = mBuffer->used / points;
    normalize = 1.0 / step;
  }

private:
  StereoRingBuffer* mBuffer;
  float lastX = 0, lastY = 0;

  int points;
  double normalize;
  int step;

  float s, c;
};

