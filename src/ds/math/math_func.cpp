#include "math_func.h"
#include "math_defs.h"

using namespace ci;

namespace ds {
namespace math {

bool intersect2D( const ci::Vec3f &start0, const ci::Vec3f &end0, const ci::Vec3f &start1, const ci::Vec3f &end1 )
{
  Vec3f u = end0 - start0;
  Vec3f v = end1 - start1;

  Vec3f w = start0 - start1;

  float bottom = ((u.dot(u))*(v.dot(v))-(u.dot(v))*(u.dot(v)));

  float t0 = 0.0f;
  float t1 = 0.0f;

  float top1 = ((u.dot(v))*(v.dot(w))-(v.dot(v))*(u.dot(w)));
  float top2 = ((u.dot(u))*(v.dot(w))-(u.dot(v))*(u.dot(w)));

  if (!isEqual(bottom, 0.0f)) {
    t0 = top1/bottom;

    t1 = top2/bottom;
  } else {
    t0 = 0.0f;

    t1 = (v.dot(w))/(v.dot(v));
  }

  t0 = clamp(t0, 0.0f, 1.0f);

  t1 = clamp(t1, 0.0f, 1.0f);

  Vec3f p1 = start0 + u * t0;

  Vec3f q1 = start1 + v * t1;

  if (isEqual(top1, 0.0f) && isEqual(top2, 0.0f)) {
    return false;
  }

  if (u.normalized() == v.normalized()) {
    return false;
  }

  if (p1 == q1) {
    return true;
  }

  return false;
}

bool isEqual( float a, float b )
{
  if (fabs(a - b) < math::EPSILON)
    return true;
  else if ((fabs(a - b)) / max(fabs(a), fabs(b)) < math::EPSILON)
    return true;
  return false;
}

bool isEqual( double a, double b )
{
  if (abs(a - b) < math::EPSILON)
    return true;
  else if ((abs(a - b)) / max(abs(a), abs(b)) < math::EPSILON)
    return true;
  return false;
}

bool isEqual( float a, double b )
{
  double da = static_cast<double>(a);
  if (abs(da - b) < math::EPSILON)
    return true;
  else if ((abs(da - b)) / max(abs(da), abs(b)) < math::EPSILON)
    return true;
  return false;
}

bool isEqual( double a, float b )
{
  double db = static_cast<double>(b);
  if (abs(a - db) < math::EPSILON)
    return true;
  else if ((abs(a - db)) / max(abs(a), abs(db)) < math::EPSILON)
    return true;
  return false;
}

double degree(const double x2, const double y2)
{
	if (y2 == 0) {
		if (x2 > 0) return 0.0;
		else if (x2 < 0) return 180.0;
		else return 0;
	}
	if (x2 == 0) {
		if (y2 > 0) return 90.0;
		else if (y2 < 0) return 270.0;
	}

	const double		x1 = 0, y1 = 0;
	const double		m1 = 0;
	const double		m2 = slope(x1, y1, x2, y2);
	const double		tangent = (m2 - m1) / (1 + (m1 * m2));
	const double		radians = atan(tangent);
	const double		degree = radians * RADIAN2DEGREE;

	if (x2 < 0 && y2 > 0) return degree + 180;
	else if (x2 < 0 && y2 < 0) return degree + 180;
	else if (x2 > 0 && y2 < 0) return degree + 360;

	return degree;
}

} // namespace math
} // namespace ds
