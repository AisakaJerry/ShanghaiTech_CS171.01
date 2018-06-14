#include "scene.h"
#include <random> 
#include "../sample/sampler.h"
#include "../objects/mesh.h"

namespace Orchid
{
	
	
	void Scene::add(Object * object)
	{
		_objects.push_back(object);
	}

	ObjectIntersection Scene::intersect(const Ray & ray)
	{
		ObjectIntersection isct = ObjectIntersection();
		ObjectIntersection temp;
		long size = _objects.size();

		for (int i = 0; i < size; i++)
		{
			temp = _objects.at((unsigned)i)->getIntersection(ray);

			if (temp._hit) {
				if (isct._u == 0 || temp._u < isct._u) isct = temp;
			}
		}
		return isct;
	}

	Vector3d Scene::trace_ray(const Ray & ray, int depth, unsigned short * Xi)
	{
		Vector3d pointcol(0.0f,0.0f,0.0f);	//initialize colour

		ObjectIntersection hitpoint = Scene::intersect(ray);

		if (hitpoint._hit)
		{
			//std::cout << "HIT" << std::endl;
			if (hitpoint._material.getType() == DIFF) {
				if (depth > 1) {
					Vector3d n_origin = ray.origin() + ray.direction() * hitpoint._u;
					Vector3d nfac = hitpoint._normal;
					nfac = nfac.normalized();
					
					double theta1 = (rand() / double(RAND_MAX) - 0.5) * 180;
					double theta2 = (rand() / double(RAND_MAX) - 0.5) * 180;
					Vector3d Nx(0.0, 0.0, 0.0);
					Vector3d Ny(0.0, 0.0, 0.0);
					Vector3d up(1.0, 1.0, 1.0);
					Nx = Vector3d::cross(nfac, up);
					Ny = Vector3d::cross(nfac, Nx);
					Nx = Nx.normalized();
					Ny = Ny.normalized();
					Vector3d n_direction = nfac + Nx * sin(theta1)*nfac.norm() + Ny * sin(theta2)*nfac.norm();
					n_direction = n_direction.normalized();			

					Ray newray(n_origin, n_direction);
					pointcol = hitpoint._material.get_colour() * trace_ray(newray, depth - 1, Xi);  // iterate
					return(pointcol);
				}
				else {
					pointcol = hitpoint._material.get_colour()*Vector3d(0.45, 0.45, 0.45);	// do not need to iterate anymore
					return(pointcol);
				}

			}
			if (hitpoint._material.getType() == EMIT) {	// reach the light source, quit
				pointcol = hitpoint._material.get_emission();
				return(pointcol);
			}
			if (hitpoint._material.getType() == SPEC) {
				if (depth > 1) {
					Vector3d n_origin = ray.origin() + ray.direction() * hitpoint._u;
					Vector3d I = ray.direction();
					Vector3d N = hitpoint._normal;
					N = N.normalized();
					I = I.normalized();
					Vector3d n_direction = -I + 2 * (Vector3d::dot(I, N)) * N;	// spec vector
					n_direction = n_direction.normalized();
					Ray newray(n_origin, n_direction);
					pointcol = hitpoint._material.get_colour() * trace_ray(newray, depth - 1, Xi);  // iterate
					return(pointcol);
				}
				else {
					pointcol = hitpoint._material.get_colour()*Vector3d(0.45, 0.45, 0.45);	// do not need to iterate anymore
					return(pointcol);
				}
			}
		}
		else {
			return Vector3d(0.0, 0.0, 0.0);   // hit nothing
		}

	}




}