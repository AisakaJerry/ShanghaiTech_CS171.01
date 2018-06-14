#include "triangle.h"

namespace Orchid
{
	Triangle::Triangle()
	{

	}
	Triangle::Triangle(const Vector3d & p0, const Vector3d & p1, const Vector3d & p2, Material material)
	{
		_p0 = p0;
		_p1 = p1;
		_p2 = p2;
		_material = material;
	}
	Triangle::Triangle(const Triangle & tri)
	{
		this->operator=(tri);
	}
	Triangle::~Triangle()
	{
	}
	Triangle & Triangle::operator=(const Triangle & tri)
	{
		// TODO: insert return statement here
		this->_p0 = tri._p0;
		this->_p1 = tri._p1;
		this->_p2 = tri._p2;
		this->_material = tri._material;
		return *this;
	}
	Vector3d Triangle::gravity() const
	{
		return Vector3d();
	}
	AABBox Triangle::get_bounding_box()
	{
		return AABBox();
	}
	bool Triangle::triangleInBbox()
	{
		return false;
	}
	Vector3d Triangle::get(int id) const
	{
		Assertion(0 <= id && id <= 2, "ID must be between 0 and 2");
		if (0 == id)
			return _p0;
		if (1 == id)
			return _p1;
		if (2 == id)
			return _p2;
	}
	Vector3d Triangle::normal() const
	{
	
	}
	ObjectIntersection Triangle::getIntersection(const Ray & ray)
	{
		Vector3d r_d = ray.direction();
		Vector3d r_o = ray.origin();
		bool hit = false;
		double t = 0;
		double b1, b2;
		Vector3d e1 = _p1 - _p0;
		Vector3d e2 = _p2 - _p0;
		Vector3d norm = Vector3d::cross(e1, e2);
		if (Vector3d::dot(norm, r_d) > 0) {	// inverse direction
			norm = -norm;
			e1 = _p2 - _p0;
			e2 = _p1 - _p0;
		}
		Vector3d s = r_o - _p0;
		double fac = 1 / (Vector3d::dot(Vector3d::cross(r_d, e2), e1));
		t = fac * Vector3d::dot(Vector3d::cross(s, e1), e2);
		b1 = fac * Vector3d::dot(Vector3d::cross(r_d, e2), s);
		b2 = fac * Vector3d::dot(Vector3d::cross(s, e1), r_d);

		if ((b1 > 0)&&(b2 > 0)&&(b1 + b2 < 1))	{
			hit = true;
		}
	
		return ObjectIntersection(hit, t, norm, _material);
	}
	double Triangle::area() const
	{
		const Vector3d e1 = _p1 - _p0;
		const Vector3d e2 = _p2 - _p0;
		return 0.5 * Vector3d::cross(e1, e2).norm();
	}
}
