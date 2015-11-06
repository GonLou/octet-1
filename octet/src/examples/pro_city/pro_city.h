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

#include "Sound.h"
#include "Engine.h"

namespace octet {
  /// Scene containing a box with octet.
  class pro_city : public app {

	// defining size 
	CONST int MAX_WIDTH = 200;
	CONST int MAX_HEIGHT = 200;
	CONST int BOUNDARIES = 10;
	CONST int MAX_SITES = 100;

    // scene for drawing box
    ref<visual_scene> app_scene;

	btDefaultCollisionConfiguration config;       /// setup for the world
	btCollisionDispatcher *dispatcher;            /// handler for collisions between objects
	btDbvtBroadphase *broadphase;                 /// handler for broadphase (rough) collision
	btSequentialImpulseConstraintSolver *solver;  /// handler to resolve collisions
	btDiscreteDynamicsWorld *world;               /// physics world, contains rigid bodies


	//dynarray<btRigidBody*> rigid_bodies;

	//dynarray<scene_node*> nodes;
	ref<material> custom_mat;

	// variables for the game 
	ref<Engine> engine;

	struct point {
		float x;
		float z;

		point() = default;

		point(float _x, float _z) {
			x = _x;
			z = _z;
		}
	};
	
	enum id { OBJ = 0, PLAYER = 1, BALL = 2, GOAL = 3 };

	struct my_objects {
		float x;
		float y;
		float z;
		float radius;
		id idc;

		my_objects() = default;

		my_objects(float _x, float _y, float _z, float _radius, id _id) {
			x = _x;
			y = _y;
			z = _z;
			radius = _radius;
			idc = _id;
		}
	};

	dynarray<my_objects> objects;

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
	//dynarray<my_edge> site_edge;
	//dynarray<point> voronoi_vertex;

	mouse_look mouse_look_helper;
	helper_fps_controller fps_helper;

	ref<scene_node> player_node;

	ref<camera_instance> the_camera;

	Sound game_sounds;

	// Overlay and text drawing
	ref<text_overlay> overlay;
	ref<mesh_text> right_text;
	ref<mesh_text> left_text;

	/// calculates distance
	static float distance(const point& vertex1, const point& vertex2) {
		int xd = vertex2.x - vertex1.x;
		int zd = vertex2.z - vertex1.z;
		return (xd * xd) + (zd * zd);
	}

	/// gives a random number
	int random_int(int min, int max) {
		if (min > max) max = min + 2;
		return rand() % (max - min + 1) + min;
	}

	/// calculates intersections
	bool do_intersect(my_edge& p1, my_edge& p2) {

		//if (p1.x1 != p2.x1 && p1.z1 != p2.z1) return false;
		//if (p1.x2 != p2.x2 && p1.z2 != p2.z2) return false;

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

	/// add vertexes
	void add_sites(mat4t &mat) {
		material *black = new material(vec4(0, 0, 0, 1));

		int count = 0;
		printf("...\nadding sites...\n");
		srand(99); //seed
		for (int i = 0; i < MAX_SITES; i++) {
			site.push_back(point(random_int(0, MAX_WIDTH - BOUNDARIES), random_int(0, MAX_HEIGHT - BOUNDARIES)));
			//printf("x: %f | z: %f\n", site[i].x, site[i].z);
			count++;
		}
		printf("... total sites %d ...\n", count);
	}

	/// draw city
	void add_cubes(mat4t &mat) {
		material *grey = new material(vec4(.5, .5, .5, 1));
		//material *white = new material(vec4(1, 1, 1, 1));
		image *img = new image("assets/pro_city/building.jpg");
		material *building = new material(img);

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
			mat.loadIdentity();
			mat.translate(p.x, 10, p.z);
			float xs = random_int(1, d / 100);
			float ys = random_int(5, 10);
			float zs = random_int(1, d / 100);
			app_scene->add_shape(mat, new mesh_box(vec3(xs, ys, zs)), building, false);
			app_scene->add_shape(mat, new mesh_box(vec3(.1, ys + random_int(1, 10), .1)), grey, false);
			//printf("distance %f\n", d);
			//printf("x1: %f | z1: %f || x2: %f | z2: %f\n", p.x, p.z, site[pos].x, site[pos].z);
			float perimeter = (xs * zs / 2);
			objects.push_back(my_objects(p.x, 10.0f, p.z, perimeter, OBJ));
			//printf("distance %f\n", perimeter);
		}
	}

  public:
    /// this is called when we construct the class before everything is initialised.
    pro_city(int argc, char **argv) : app(argc, argv) {
		dispatcher = new btCollisionDispatcher(&config);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver();
		world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, &config);
    }

	~pro_city() {
		delete world;
		delete solver;
		delete broadphase;
		delete dispatcher;
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

		world->setGravity(btVector3(0, -100.0f, 0));

		material *red = new material(vec4(1, 0, 0, 1));
		material *green = new material(vec4(0, 1, 0, 1));	
	  
		mat4t mat;

		// ground
		{
			printf("...\nmaking ground...\n");
			mat.loadIdentity();
			mat.translate(0, 5, 0);
			app_scene->add_shape(mat, new mesh_box(vec3(MAX_WIDTH*10, 1, MAX_HEIGHT*10)), green, false);
		}

		add_sites(mat);

		add_cubes(mat);

		// gate
		{
			mat.loadIdentity();
			mat.translate(150, 10, 250);
			app_scene->add_shape(mat, new mesh_box(vec3(.5, 40, .5)), red, false);
			mat.translate(20, 0, 0);
			app_scene->add_shape(mat, new mesh_box(vec3(.5, 40, .5)), red, false);
			mat.translate(-10, 40, 0);
			mat.rotateX90();
			mat.rotateY90();
			app_scene->add_shape(mat, new mesh_box(vec3(.5, 20, .5)), green, false);

			objects.push_back(my_objects(150, 10, 250, 1, GOAL));
		}

	  // player from example_fps.h
	  {
		  printf("...\ndeploying player...\n");
		  float player_height = 50.00f;
		  float player_radius = 0.25f;
		  float player_mass = 90.0f;

		  mat.loadIdentity();
		  mat.rotateY(90);
		  mat.translate(100, player_height*0.5f, 0);
		  mat.rotateZ(90);

		  mesh_instance *mi = app_scene->add_shape(
			  mat,
			  new mesh_sphere(vec3(0), player_radius),
			  new material(vec4(0, 0, 1, 1)),
			  true, player_mass,
			  new btCapsuleShape(0.25f, 2)
			  );
		  player_node = mi->get_node();

		  objects.push_back(my_objects(100, player_height*0.5f, 0, 1, PLAYER));
	  }

	  // just a transparent ball
	  {
		  mat.loadIdentity();
		  mat.translate(50, 12, -5);
		  param_shader *shader = new param_shader("shaders/default.vs", "shaders/default_solid_transparent.fs");
		  custom_mat = new material(vec4(1, 1, 1, 1), shader);
		  app_scene->add_shape(mat, new mesh_sphere(vec3(0, 0, 0), 1), custom_mat, true);
		  objects.push_back(my_objects(50, 12, -5, 1, BALL));
	  }

	  // text from example_text.h
	  {
		  printf("...\nwriting text...\n");

		  overlay = new text_overlay();

		  // get the font
		  bitmap_font *font = overlay->get_default_font();

		  // create a box containing text (in pixels)
		  aabb Rtext_aabb(vec3(600.0f, 150.0f, 0.0f), vec3(256, 128, 0));
		  aabb Ltext_aabb(vec3(-150.0f, 150.0f, 0.0f), vec3(256, 128, 0));
		  right_text = new mesh_text(font, "Score: 10000", &Rtext_aabb);
		  left_text = new mesh_text(font, "Mission #1: Head to ball", &Ltext_aabb);

		  // add the mesh to the overlay.
		  overlay->add_mesh_text(right_text);
		  overlay->add_mesh_text(left_text);
	  }

	  // sounds from class
	  {
		  printf("...\ninitialize sounds...\n");
		  game_sounds.init_sound();
	  }

	  engine = new Engine(10, Engine::State::HALL_OF_FAME);
	  //core.gameMode = Core::INIT;

	  printf("engine %d\n", engine->GetState());
	
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  switch (engine->GetState()) {
		case (Engine::State::GAME_START) : 
			break;
		case (Engine::State::GAME_END) :
			game_sounds.playSoundEnd();
			break;
		case (Engine::State::HALL_OF_FAME) :
			left_text->clear();
			left_text->format("press <SPACE> to start\n");
			break;	
		default :
			printf("something went wrong...\n");
			break;
	  }

	  scene_node *cam_node = app_scene->get_camera_instance(0)->get_node();
	  mat4t &cam_to_world = cam_node->access_nodeToParent();

	  mouse_look_helper.update(cam_to_world);

      fps_helper.update(player_node, cam_node);

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

	  // convert it to a mesh.
	  right_text->update();
	  left_text->update();

	  // draw the text overlay
	  overlay->render(vx, vy);

    }
  };
}
