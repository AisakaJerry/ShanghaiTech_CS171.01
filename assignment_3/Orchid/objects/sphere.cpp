#include "sphere.h"
namespace  Orchid
{
	Sphere::Sphere(Vector3d position, double radius, Material material)

	{
		_position = position;
		_radius = radius;
		_material = material;
	}
	ObjectIntersection Sphere::getIntersection(const Ray & ray)
	{
		double dx = ray.direction()[0];
		double dy = ray.direction()[1];
		double dz = ray.direction()[2];
		double ox = ray.origin()[0];
		double oy = ray.origin()[1];
		double oz = ray.origin()[2];
		double cx = _position[0];
		double cy = _position[1];
		double cz = _position[2];
		double r = _radius;

		double A = dx * dx + dy * dy + dz * dz;
		double B = (2 * ox*dx - 2 * cx*dx + 2 * oy*dy - 2 * cy*dy + 2 * oz*dz - 2 * cz*dz);
		double C = (ox * ox + oy * oy + oz * oz + cx * cx + cy * cy + cz * cz - 2 * (ox * cx + oy * cy + oz * cz) - r * r);

		if (B * B - 4 * A * C < 0) {
			return ObjectIntersection(false, 0, Vector3d(0.0), Material(DIFF));	// no intersection
		}
		else {
			double t = (-B - sqrt(B*B - 4 * A*C)) / (2 * A);
			Vector3d hitpoint = ray.origin() + t * ray.direction();
			Vector3d nvec = hitpoint - _position;
			nvec = nvec.normalized();
			return ObjectIntersection(true, t, -nvec, _material);
		}


	}
}
