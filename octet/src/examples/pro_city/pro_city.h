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
	CONST int BOUNDARIES = 100;
	CONST int MAX_SITES = 100;

    // scene for drawing box
    ref<visual_scene> app_scene;

	struct point {
		float x;
		float z;

		point() = default;

		point(float _x, float _z) {
			x = _x;
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

	dynarray<point> site;
	//dynarray<my_edge> temp_site_edge;
	dynarray<my_edge> site_edge;
	dynarray<point> voronoi_vertex;


	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;

	// calculates neighbours
	static int square_distance(const point& vertex, int x, int z) {
		int xd = x - vertex.x;
		int zd = z - vertex.z;
		return (xd * xd) + (zd * zd);
	}

	// calculates distance
	static float distance(const point& vertex1, const point& vertex2) {
		int xd = vertex2.x - vertex1.x;
		int zd = vertex2.z - vertex1.z;
		return (xd * xd) + (zd * zd);
	}

	// gives a random number
	int random_int(int min, int max) {
		return rand() % (max - min + 1) + min;
	}

	bool do_intersect(my_edge& p1, my_edge& p2)
		/*bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
			float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)*/
	{

		if (p1.x1 != p2.x1 && p1.z1 != p2.z1) return false;
		if (p1.x2 != p2.x2 && p1.z2 != p2.z2) return false;

		float s1_x, s1_y, s2_x, s2_y;
		s1_x = p1.x2 - p1.x1;     
		s1_y = p1.z2 - p1.z1;
		s2_x = p2.x2 - p2.x1;     
		s2_y = p2.z2 - p2.z1;

		float s, t;
		s = (-s1_y * (p1.x1 - p2.x1) + s1_x * (p1.z1 - p2.z1)) / (-s2_x * s1_y + s1_x * s2_y);
		t = (s2_x * (p1.z1 - p2.z1) - s2_y * (p1.x1 - p2.x1)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			// Collision detected
			//if (i_x != NULL)
			float i_x = p1.x1 + (t * s1_x);
			//if (i_y != NULL)
			float i_y = p1.z1 + (t * s1_y);
			printf("collision at %f,%f\n", i_x, i_y);
			return true;
		}

		return false; // No collision
	}

	void add_sites(mat4t &mat) {
		material *black = new material(vec4(0, 0, 0, 1));

		int count = 0;
		printf("...\nadding sites...\n");
		srand(99);
		for (int i = 0; i < MAX_SITES; i++) {
			site.push_back(point(random_int(0, MAX_WIDTH - BOUNDARIES), random_int(0, MAX_HEIGHT - BOUNDARIES)));
			//printf("x: %f | z: %f\n", site[i].x, site[i].z);
			// adding sites as spheres
			mat.loadIdentity();
			mat.translate(site[i].x, 100, site[i].z);
			app_scene->add_shape(mat, new mesh_sphere(vec3(0, 0, 0), .5), black, true);
			count++;
		}
		printf("... total sites %d ...\n", count);
	}

	void add_cubes(mat4t &mat) {
		material *green = new material(vec4(1, 0, 0, 1));

		printf("...\nmaking rectangles on each site...\n");
		for (size_t it = 0; it < site.size(); it++) {
			const point& p = site[it];
			float d = CL_MAXFLOAT;
			int pos = 0;
			for (size_t it2 = 0; it2 < site.size(); it2++) {
				const point& p2 = site[it2];
				float dd = distance(p, p2);
				if (dd < d && dd > 0) {
					d = dd;
					pos = it2;
				}
				//printf("temp distance %f\n", dd);

			}
			//site_edge.push_back(my_edge(p.x, p.z, site[pos].x, site[pos].z));
			mat.loadIdentity();
			mat.translate(p.x, 10, p.z);
			//mat.rotate(5,1,0,0);
			app_scene->add_shape(mat, new mesh_box(vec3(1, 10, 1)), green, false);

			//printf("distance %f\n", d);
			//printf("x1: %f | z1: %f || x2: %f | z2: %f\n", p.x, p.z, site[pos].x, site[pos].z);
		}
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
		material *white = new material(vec4(1, 1, 1, 1));
	  
		mat4t mat;

		// ground
		mat.loadIdentity();
		mat.translate(0, -1, 0);
		//mat.rotate(5,1,0,0);
		app_scene->add_shape(mat, new mesh_box(vec3(MAX_WIDTH*10, 1, MAX_HEIGHT*10)), green, false);

		// just a ball
		mat.loadIdentity();
		mat.translate(0, 12, 0);
		app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

		add_sites(mat);

		add_cubes(mat);

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
