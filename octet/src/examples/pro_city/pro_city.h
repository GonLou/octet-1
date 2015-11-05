////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace octet {
  /// Scene containing a box with octet.
  class pro_city : public app {

	// defining size 
	CONST int MAX_WIDTH = 200;
	CONST int MAX_HEIGHT = 200;
	CONST int BOUNDARIES = 50;

    // scene for drawing box
    ref<visual_scene> app_scene;

	struct my_coords {
		float x;
		float y;
		float z;

		my_coords() = default;

		my_coords(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};

	struct my_edge {
		float x1;
		float z1;
		float x2;
		float z2;

		my_edge() = default;

		my_edge(float _x1, float _z1, float _x2, float _z2) {
			x1 = _x1;
			z1 = _z1;
			x2 = _x2;
			z2 = _z2;
		}
	};

	dynarray<my_coords> site;
	dynarray<my_edge> temp_site_edge;
	dynarray<my_edge> final_site_edge;
	dynarray<my_coords> voronoi_vertex;


	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;

	// calculates neighbours
	static int square_distance(const my_coords& vertex, int x, int z) {
		int xd = x - vertex.x;
		int zd = z - vertex.z;
		return (xd * xd) + (zd * zd);
	}

	// gives a random number
	int random_int(int min, int max) {
		return rand() % (max - min + 1) + min;
	}

  public:
    /// this is called when we construct the class before everything is initialised.
    pro_city(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
	void app_init() {
		mouse_look_helper.init(this, 200.0f / 360.0f, false);
		fps_helper.init(this);
		app_scene =  new visual_scene();
		app_scene->create_default_camera_and_lights();
		the_camera = app_scene->get_camera_instance(0);
		the_camera->get_node()->translate(vec3(0, 4, 0));
		the_camera->get_node()->rotate(180, vec3(0, 180, 0));
		the_camera->set_far_plane(10000);

		material *red = new material(vec4(1, 0, 0, 1));
		material *green = new material(vec4(0, 1, 0, 1));
		material *black = new material(vec4(0, 0, 0, 1));
		material *white = new material(vec4(1, 1, 1, 1));
	  
		mat4t mat;

		// ground
		mat.loadIdentity();
		mat.translate(0, -1, 0);
		//mat.rotate(5,1,0,0);
		app_scene->add_shape(mat, new mesh_box(vec3(MAX_WIDTH, 1, MAX_HEIGHT)), green, false);

		// just a ball
		mat.translate(0, 12, 0);
		app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

		int count = 0;
		printf("...\nadding sites...\n");
		srand(100);
		for (int i = 0; i < 10; i++) {
			site.push_back(my_coords(random_int(0, MAX_WIDTH - BOUNDARIES), 0, random_int(0, MAX_HEIGHT - BOUNDARIES)));
			printf("x: %f | z: %f\n",  site[i].x,  site[i].z);
			// adding sites as spheres
			mat.translate(site[i].x, site[i].y, site[i].z);
			app_scene->add_shape(mat, new mesh_sphere(vec3(0, 0, 0), .2), black, true);
			count++;
		}
		printf("... total sites %d ...\n", count);

		count = 0;
		printf("...\nconnecting sites with lines...\n");
		for (size_t it = 0; it < site.size(); it++) {
			const my_coords& p = site[it];
			for (size_t it2 = 1 + it; it2 < site.size(); it2++) {
				const my_coords& p2 = site[it2];
				temp_site_edge.push_back(my_edge(p.x, p.z, p2.x, p2.z));
				printf("x1: %f | z1: %f || x2: %f | z2: %f\n", p.x, p.z, p2.x, p2.z);
				count++;
			}
		}
		printf("... total temporary lines %d ...\n", count);

		count = 0;
		printf("...\ncheck lines that intersects others and delete them...\n"); // kind of brute-force method here
		for (size_t it = 0; it < temp_site_edge.size(); it++) {
			const my_edge& e = temp_site_edge[it];
			for (size_t it2 = 0 + it; it2 < temp_site_edge.size(); it2++) {
				const my_edge& e2 = temp_site_edge[it2];
				if ((((e2.z2 - e2.z1) / (e2.x2 - e2.x1)) - ((e.z2 - e.z1) / (e.x2 - e.x1))) == 0) { // intersection formula
					final_site_edge.push_back(my_edge(e.x1, e.z1, e.x2, e.z2));
					printf("x1: %f | z1: %f || x2: %f | z2: %f\n", e.x1, e.z1, e.x2, e.z2);
					count++;
				}
			}
		}
		printf("... total final lines %d ...\n", count);

	//printf(".\n showing lines...\n");
	//mat.loadIdentity();
	//mat.translate(0, -1, 0);
	////mat.rotate(5,1,0,0);
	//app_scene->add_shape(mat, new mesh_box(vec3(MAX_WIDTH, 1, MAX_HEIGHT)), green, false);


	//int nearest_site(double x, double y)
	//{
	// int k, ret = 0;
	// double d, dist = 0;
	// #define for_k for (k = 0; k < N_SITES; k++)
	// for_k{
	//  d = sq2(x - site[k][0], y - site[k][1]);
	// if (!k || d < dist) {
	//  dist = d, ret = k;
	// }
	// }
	// return ret;
	//}

	//int nearest_site(double x, double y)
	//{
	// int k, ret = 0;
	// double d, dist = 0;
	// #define for_k for (k = 0; k < N_SITES; k++)
	// for_k{
	//  d = sq2(x - site[k][0], y - site[k][1]);
	// if (!k || d < dist) {
	//  dist = d, ret = k;
	// }
	// }
	// return ret;
	//}
	//int w = MAX_WIDTH - BOUNDARIES, h = MAX_HEIGHT - BOUNDARIES, d;
	//for (int hh = 0; hh < h; hh++) {
	//	for (int ww = 0; ww < w; ww++) {
	//		int ind = -1, dist = INT_MAX;
	//		for (size_t it = 0; it < site.size(); it++) {
	//			const my_coords& p = site[it];
	//			d = square_distance(p, ww, hh);
	//			if (d < dist) {
	//				dist = d;
	//				ind = it;
	//			}
	//		}

	//		if (ind > -1) {
	//			voronoi_vertex.push_back((my_coords(ww, 0, hh)));
	//			//SetPixel(bmp_->hdc(), ww, hh, colors_[ind]);
	//		}
	//		else
	//			printf("out of boundaries!");
	//	}
	//}

	printf("adding voronoi vertex...\n");
	// formula to calculate if both lines intersects
	//n.x * (P.x - P_0.x) + n.y * (P.y - P_0.y) + n.z * (P.z - P_0.z) = 0;


	  // player from example_fps.h
	  {
	  float player_height = 100.83f;
	  float player_radius = 0.25f;
	  float player_mass = 90.0f;

	  mat.loadIdentity();
	  mat.translate(0, player_height*0.5f, -50);

	  mesh_instance *mi = app_scene->add_shape(
		  mat,
		  new mesh_sphere(vec3(0), player_radius),
		  new material(vec4(0, 0, 1, 1)),
		  true, player_mass,
		  new btCapsuleShape(0.25f, player_height)
		  );
	  player_node = mi->get_node();
	  }

    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  scene_node *cam_node = app_scene->get_camera_instance(0)->get_node();
	  mat4t &cam_to_world = cam_node->access_nodeToParent();

	  mouse_look_helper.update(cam_to_world);

      fps_helper.update(player_node, cam_node);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

    }
  };
}
