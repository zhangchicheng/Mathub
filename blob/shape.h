#ifndef SHAPE_H
#define SHAPE_H

namespace Mathub
{
  template <unsigned N>
  struct Shape
  {
    unsigned shape[N] = {};
    const unsigned& operator[](unsigned i) const { return shape[i]; }
  };

  template <>
  struct Shape<0>
  {
    unsigned shape = {};
  };
}

#endif
