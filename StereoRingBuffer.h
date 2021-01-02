#pragma once
#include <malloc.h>

struct StereoRingBuffer {
  double* left = nullptr;
  double* right = nullptr;

  size_t allocated = 0;
  size_t used = 0;
  size_t position = 0;
};

inline StereoRingBuffer RingBufferSolidify(StereoRingBuffer* buffer) {
  StereoRingBuffer out;
  out.used = buffer->used;
  out.allocated = buffer->used;
  size_t length = buffer->used * sizeof(double);
  out.left = (double*)malloc(length);
  out.right = (double*)malloc(length);
  for (size_t i = 0; i < buffer->used; i++) {
#pragma warning(disable: 6386)
    out.left[i] = buffer->left[(i + buffer->position) % buffer->used];
    out.right[i] = buffer->right[(i + buffer->position) % buffer->used];
#pragma warning(default: 6386)
  }
  out.position = 0;
  return out;
}

inline void RingBufferResize(StereoRingBuffer* buffer, size_t samples) {
  if (buffer->allocated >= samples) {
    buffer->used = samples;
    return;
  }

  if (buffer->left) {
    free(buffer->left);
  }
  if (buffer->right) {
    free(buffer->right);
  }

  size_t length = sizeof(double) * samples;
  buffer->left = (double*)malloc(length);
  buffer->right = (double*)malloc(length);
  memset(buffer->left, 0, length);
  memset(buffer->right, 0, length);
  buffer->used = samples;
  buffer->allocated = samples;
}

inline void RingBufferWrite(StereoRingBuffer* buffer, double left, double right) {
  size_t pos = buffer->position % buffer->used;
  buffer->left[pos] = left;
  buffer->right[pos] = right;
  buffer->position = pos + 1;
}
