#include "mesh.h"
#include "../accel/aabbox.h"
#include <math.h>

namespace Orchid
{
	Mesh::Mesh(Vector3d p_, const char * file_path, Material m_)
	{
		_p = p_, m_m = m_;

		std::string mtlbasepath;
		std::string inputfile = file_path;
		unsigned long pos = inputfile.find_last_of("/");
		mtlbasepath = inputfile.substr(0, pos + 1);

		printf("Loading %s...\n", file_path);
		// Attempt to load mesh
		std::string err = tinyobj::LoadObj(m_shapes, m_materials, inputfile.c_str(), mtlbasepath.c_str());

		if (!err.empty()) {
			std::cerr << err << std::endl;
			exit(1);
		}
		printf(" - Generating k-d tree...\n\n");

		long shapes_size, indices_size, materials_size;
		shapes_size = m_shapes.size();
		materials_size = m_materials.size();

		// Load materials/textures from obj
		// TODO: Only texture is loaded at the moment, need to implement material types and colours
		for (int i = 0; i < materials_size; i++) {
			std::string texture_path = "";

			if (!m_materials[i].diffuse_texname.empty()) {
				if (m_materials[i].diffuse_texname[0] == '/') texture_path = m_materials[i].diffuse_texname;
				texture_path = mtlbasepath + m_materials[i].diffuse_texname;
				materials.push_back(Material(DIFF, Vector3d(1, 1, 1), Vector3d(), texture_path.c_str()));
			}
			else {
				materials.push_back(Material(DIFF, Vector3d(1, 1, 1), Vector3d()));
			}

		}

		// Load triangles from obj
		for (int i = 0; i < shapes_size; i++) {
			indices_size = m_shapes[i].mesh.indices.size() / 3;
			for (size_t f = 0; f < indices_size; f++) {

				// Triangle vertex coordinates
				Vector3d v0_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f] * 3 + 2]
				) + _p;

				Vector3d v1_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 1] * 3 + 2]
				) + _p;

				Vector3d v2_ = Vector3d(
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 1],
					m_shapes[i].mesh.positions[m_shapes[i].mesh.indices[3 * f + 2] * 3 + 2]
				) + _p;

				Vector3d t0_, t1_, t2_;

				//Attempt to load triangle texture coordinates
				if (m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1 < m_shapes[i].mesh.texcoords.size()) {
					t0_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f] * 2 + 1],
						0
					);

					t1_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 1] * 2 + 1],
						0
					);

					t2_ = Vector3d(
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2],
						m_shapes[i].mesh.texcoords[m_shapes[i].mesh.indices[3 * f + 2] * 2 + 1],
						0
					);
				}
				else {
					t0_ = Vector3d();
					t1_ = Vector3d();
					t2_ = Vector3d();
				}
				
				Vector3d offset(250.0, 250.0, 250.0);
				v0_ = v0_ * 10;
				v1_ = v1_ * 10;
				v2_ = v2_ * 10;
				v0_ += offset;
				v1_ += offset;
				v2_ += offset;
		
				

				if (m_shapes[i].mesh.material_ids[f] < materials.size()) {
					tris.push_back(new Triangle(v0_, v1_, v2_, materials[m_shapes[i].mesh.material_ids[f]]));

				}
				else {
					tris.push_back(new Triangle(v0_, v1_, v2_, m_m));
				}
			}
		}

		// Clean up
		m_shapes.clear();
		m_materials.clear();
		printf("\n");

	}
	ObjectIntersection Mesh::getIntersection(const Ray & r)
	{	
		
		int boxindex[2][2][2][1000];
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					for (int l = 0; l < 1000; l++) {
						boxindex[i][j][k][l] = -1;	// initialization as -1
					}
				}
			}
		}
		int xmin, xmax, ymin, ymax, zmin, zmax;
		AABBox origin_box = AABBox(Vector3d(0.0, 0.0, 560.0), Vector3d(560.0, 560.0, 0.0));	// largest aabb
		for (int i = 0; i < tris.size(); i++) {
			Triangle *tri = tris.at((unsigned)i);
			AABBox tribox = AABBox(tri->get_bounding_box());
			xmin = floor(tribox.bl[0] / 560 * 2);
			xmax = floor(tribox.tr[0] / 560 * 2);
			ymin = floor(tribox.bl[1] / 560 * 2);
			ymax = floor(tribox.tr[1] / 560 * 2);
			zmin = floor(tribox.tr[2] / 560 * 2);
			zmax = floor(tribox.bl[2] / 560 * 2);
			for (int indx = xmin; indx <= xmax; indx++) {
				for (int indy = ymin; indy <= ymax; indy++) {
					for (int indz = zmin; indz <= zmax; indz++) {
						int find = 0;
						while (boxindex[indx][indy][indz][find] != -1) find++;
						boxindex[indx][indy][indz][find] = i;
					}
				}
			}
		}
		
		double t = 0, tmin = INFINITY;
		double dist = 0;
		Vector3d normal = Vector3d();
		Vector3d colour = Vector3d();
		ObjectIntersection isct = ObjectIntersection(false, tmin, normal, Material(DIFF, colour, Vector3d()));
		
		AABBox sbox[8];
		if (origin_box.intersection(r, dist)) {
			sbox[0] = AABBox(Vector3d(0, 280, 280), Vector3d(280, 560, 0));
			sbox[1] = AABBox(Vector3d(280, 280, 280), Vector3d(560, 560, 0));
			sbox[2] = AABBox(Vector3d(0, 280, 560), Vector3d(280, 560, 280));
			sbox[3] = AABBox(Vector3d(280, 280, 560), Vector3d(560, 560, 280));
			sbox[4] = AABBox(Vector3d(0, 0, 280), Vector3d(280, 280, 0));
			sbox[5] = AABBox(Vector3d(280, 0, 280), Vector3d(560, 0, 0));
			sbox[6] = AABBox(Vector3d(0, 0, 560), Vector3d(280, 280, 280));
			sbox[7] = AABBox(Vector3d(280, 0, 560), Vector3d(560, 280, 280));
			for (int i = 0; i < 8; i++) {
				int indx, indy, indz;
				switch (i) {
				case 0: {
					indx = 1; indy = 2; indz = 1;
				}
				case 1: {
					indx = 2; indy = 2; indz = 1;
				}
				case 2: {
					indx = 1; indy = 2; indz = 2;
				}
				case 3: {
					indx = 2; indy = 2; indz = 2;
				}
				case 4: {
					indx = 1; indy = 1; indz = 1;
				}
				case 5: {
					indx = 2; indy = 1; indz = 1;
				}
				case 6: {
					indx = 1; indy = 1; indz = 2;
				}
				case 7: {
					indx = 2; indy = 1; indz = 2;
				}
				}
				if (sbox[i].intersection(r, dist)) {
					ObjectIntersection temp;
					int find = 0;
					while (boxindex[indx-1][indy-1][indz-1][find] != -1) 
						find++;
					for (int j = 0; j < find; j++) {
						Triangle *tri = tris.at((unsigned)boxindex[indx-1][indy-1][indz-1][j]);
						temp = tri->getIntersection(r);
						if (temp._hit) {
							//std::cout << "HIT" << std::endl;
							if (temp._u < isct._u) isct = temp; // choose the closest one
						}
					}
				}
			}
		}
		isct._normal = -isct._normal;
		return isct;
		


		/*
		ObjectIntersection temp;
		for (int i = 0; i < tris.size(); i++) {
			Triangle *tri = tris.at((unsigned)i);
			temp = tri->getIntersection(r);
			if (temp._hit) {
				//std::cout << "HIT" << std::endl;
				if (temp._u < isct._u) isct = temp; // choose the closest one
			}
		}
		isct._normal = -isct._normal;
		return isct;
		*/
		
	}
}
