#include "quad.h"
#include "triangle.h"

namespace Orchid
{
	Quad::Quad(
		const Vector3d & a,
		const Vector3d & b,
		const Vector3d & c,
		const Vector3d & d,
		const Material & material)
		: _a{ a }
		, _b{ b }
		, _c{ c }
		, _d{ d }
		, _material{ material }
	{
	}
	ObjectIntersection Quad::getIntersection(const Ray & ray)
	{
		Triangle Tri1(_a, _b, _c, _material);
		Triangle Tri2(_a, _c, _d, _material);
		ObjectIntersection int1 = Tri1.getIntersection(ray);
		if (int1._hit) {
			return int1;
		}
		else {
			ObjectIntersection int2 = Tri2.getIntersection(ray);
			if (int2._hit) {
				return int2;
			}
			else return ObjectIntersection(false, 0, Vector3d(0.0), Material(DIFF));
		}
	}
}
