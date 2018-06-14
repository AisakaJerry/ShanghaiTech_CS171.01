#include "camera.h"
#include <stdlib.h>


namespace Orchid
{
	Camera::Camera
	(
		Vector3d position, Vector3d target, Vector3d up,
		int width, int height,
		double nearPalneDistance, double fov
	) :	_position{ position },
		_up{ up },
		_imageW{ width },
		_imageH{ height },
		_nearPlaneDistance{ nearPalneDistance }
	{
		_cameraVerticalFOV = fov *PI / 180.0f;
		_aspectRatio = (double)_imageW / _imageH;
		_cameraFwd = (target - _position).normalized();
		_cameraRight = _up.cross(_cameraFwd).normalized();
		_cameraUp = _cameraFwd.cross(_cameraRight).normalized()*(-1);
		_cameraHorizFOV = _cameraVerticalFOV *_aspectRatio;
		_windowTop = tan(_cameraVerticalFOV / 2.0f)*_nearPlaneDistance;
		_windowRight = tan(_cameraHorizFOV / 2.0f)*_nearPlaneDistance;
	}

	Ray Camera::get_ray(int x, int y, bool jitter, unsigned short * Xi)
	{

		float imageAspectRatio = _imageW / (float)_imageH; // assuming width > height 
		Vector3d Px = _cameraRight * (2 * ((x + rand() / double(RAND_MAX)) / _imageW) - 1) * tan(_cameraVerticalFOV / 2) *_nearPlaneDistance;
		Vector3d Py = _cameraUp * (1 - 2 * ((y + rand() / double(RAND_MAX)) / _imageH)) * tan(_cameraHorizFOV / 2) *_nearPlaneDistance;
		Vector3d rayDirection =(Px + Py + _position + _nearPlaneDistance * _cameraFwd) - _position;
		// note that this just equal to Vec3f(Px, Py, -1); 
		rayDirection = rayDirection.normalized(); // it's a direction so don't forget to normalize 		
		Vector3d rayOrigin = _position;
		Ray get_ray(rayOrigin, rayDirection);
		return get_ray;
	}

}
