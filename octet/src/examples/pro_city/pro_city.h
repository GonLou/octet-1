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
    // scene for drawing box
    ref<visual_scene> app_scene;

	dynarray<vec3p> sites;


  public:
    /// this is called when we construct the class before everything is initialised.
    pro_city(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();
	  app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 4, 0));
	  //app_scene->get_camera_instance(0)->get_node()->translate(vec3(0, 120, 0));
	  //app_scene->get_camera_instance(0)->get_node()->rotate(-45,vec3(10, 0, 0));

      material *red = new material(vec4(1, 0, 0, 1));
	  material *brown = new material(vec4(0, 1, 0, 1));
	  material *white = new material(vec4(0, 0, 0, 1));
	  material *black = new material(vec4(1, 1, 1, 1));
	  
	  mat4t mat;

	  // ground
	  mat.loadIdentity();
	  mat.translate(0, -1, 0);
	  //mat.rotate(5,1,0,0);
	  app_scene->add_shape(mat, new mesh_box(vec3(200, 1, 200)), brown, false);

	  // just a ball
	  mat.translate(0, 12, 0);
	  app_scene->add_shape(mat, new mesh_sphere(vec3(2, 2, 2), 2), red, true);

	  printf("adding sites...\n");
	  srand(100);
	  for (int i = 0; i < 10; i++) {
		  //sites.push_back(vec3(rand() % 100, 0, rand() % 100));
		  int x = rand() % 100;
		  int z = rand() % 100;
		  printf("x: %d | z: %d\n", x, z);
		  // adding sites as cylinders
		  //sites.end;
		  mat.translate(x, 0, z);
		  app_scene->add_shape(mat, new mesh_cylinder(vec3(2, 2, 2), 2), red, true);
	  }

	  //polygon
	  //polygon top_level;
	  //top_level.add_vertex(vec3(-1000, 0, -1000));
	  //top_level.add_vertex(vec3(1000, 0, -1000));
	  //top_level.add_vertex(vec3(1000, 0, 1000));
	  //top_level.add_vertex(vec3(-1000, 0, 1000));
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      int vx = 0, vy = 0;
      get_viewport_size(vx, vy);
      app_scene->begin_render(vx, vy);

	  scene_node *cam_node = app_scene->get_camera_instance(0)->get_node();


	  {
		  


		  if (app::is_key_down(key_up)) {
			  cam_node->translate(vec3(0, 0, -1));
		  }
		  if (app::is_key_down(key_down)) {
			  cam_node->translate(vec3(0, 0, 1));
		  }
		  if (app::is_key_down(key_left)) {
			  cam_node->rotate(5, vec3(0, 1, 0));
		  }
		  if (app::is_key_down(key_right)) {
			  cam_node->rotate(-5, vec3(0, 1, 0));
		  }


	  }

      // update matrices. assume 30 fps.
      app_scene->update(1.0f/30);

      // draw the scene
      app_scene->render((float)vx / vy);

      // tumble the box  (there is only one mesh instance)
      //scene_node *node = app_scene->get_mesh_instance(0)->get_node();

    }
  };
}
