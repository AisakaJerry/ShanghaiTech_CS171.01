#include "renderer.h"
#include <vector>
#include <stdio.h>
#include <iostream>
#include <omp.h>
#include "../3rdParty/loadpng/lodepng.h"
namespace Orchid
{


	// Clamp double to min/max of 0/1
	inline double clamp(double x) { return x < 0 ? 0 : x>1 ? 1 : x; }
	// Clamp to between 0-255
	inline int toInt(double x) { return int(clamp(x) * 255 + .5); }

	Renderer::Renderer(Scene * scene, Camera * camera)
	{
		_scene = scene;
		_camera = camera;
		_pixelBuffer = new Vector3d[_camera->imageW()*_camera->imageH()];
	}
	void Renderer::render(int samples)
	{
		int width = _camera->imageW();
		int height = _camera->imageH();
		double samples_recp = 1. / samples;

		// Main Loop
#pragma omp parallel for schedule(dynamic, 1)       // OpenMP
		for (int y = 0; y < height; y++) 
		{
			unsigned short Xi[3] = { 0,0,y*y*y };               // Stores seed for erand48
			fprintf(stderr, "\rRendering (%i samples): %.2f%% ",      // Prints
				samples, (double)y / height * 100);                   // progress

			for (int x = 0; x < width; x++) 
			{
				Vector3d col = Vector3d();

				for (int a = 0; a < samples; a++)
				{
					Ray ray = _camera->get_ray(x, y, a > 0, Xi);
					col = col + _scene->trace_ray(ray, 5, Xi);
				}
				_pixelBuffer[(y)*width + x] = col * samples_recp;
			}
		}
	}
	void Renderer::saveImage(const char * filename)
	{
		int width = _camera->imageW();
		int height = _camera->imageH();
		std::vector<unsigned char> pixel_buffer;
		int pixel_count = width*height;

		for (int i = 0; i < pixel_count; i++) {
			pixel_buffer.push_back(toInt(_pixelBuffer[i].x()));
			pixel_buffer.push_back(toInt(_pixelBuffer[i].y()));
			pixel_buffer.push_back(toInt(_pixelBuffer[i].z()));
			pixel_buffer.push_back(255);
		}
		//Encode the image
		unsigned error = lodepng::encode(filename, pixel_buffer, width, height);
		//if there's an error, display it
		if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		pixel_buffer.clear();
	}
}
