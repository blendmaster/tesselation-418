
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include <iostream>
#include <cstdlib>

#include <mesh.h>
#include <trackballhandler.h>
#include <program.h>
#include <buffer.h>
#include <glutwrapper.h>
#include <menucreator.h>

using namespace std;
using namespace glm;
using namespace EZGraphics;

/* -------------------------------------- */

class ViewerEventHandlers : public TrackballHandler, public MenuCreator {

  Program *p;
  vec3 mx, mn;
  float maxdim;
  vec3 center;
  IndexBuffer *ix;
  Buffer *cpts;
  VertexArray *va;
  static int router,rinner;


public:

  ViewerEventHandlers ( int argc, char **argv ) :
    TrackballHandler(argc,argv,GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGB,800,800)
  {
  }

/* -------------------------------------- */


  static void seti1()
  {
    rinner = 1;
  }

  static void seti2()
  {
    rinner = 2;
  }

  static void seti4()
  {
    rinner = 4;
  }

  static void seti8()
  {
    rinner = 8;
  }

  static void seti16()
  {
    rinner = 16;
  }

  static void seti32()
  {
    rinner = 32;
  }

  static void seti64()
  {
    rinner = 64;
  }

  static void seto1()
  {
    router = 1;
  }

  static void seto2()
  {
    router = 2;
  }

  static void seto4()
  {
    router = 4;
  }

  static void seto8()
  {
    router = 8;
  }

  static void seto16()
  {
    router = 16;
  }

  static void seto32()
  {
    router = 32;
  }

  static void seto64()
  {
    router = 64;
  }

  /* -------------------------------------- */

  void initializeGL()
  {

    /* TessE = tesselation evaluation, TessC = tesselation control */

    p = createProgram(ShaderFile(Vert,"shaders/vtx.glsl"),
		      ShaderFile(TessC,"shaders/ctrl.glsl"),
		      ShaderFile(TessE,"shaders/eval.glsl"),
		      ShaderFile(Geom,"shaders/geom.glsl"),
    		      ShaderFile(Frag,"shaders/frg.glsl"));
    
    /* You'll need to copy the control point locations for the teapot here */

    const GLfloat vc[4*3] = {
      -1,-1,-1,
      1,-1,-1,
      1,1,-1,
      -1,1,-1,
    };  

    /* ... and the patch indices here */

    const GLuint ixa[4] = {
      0,1,3,2
    };

    /* usual buffer, index buffer and vertex array construction;
       you should be able to use the same code with altered sizes */

    cpts = new Buffer(4,(vec3*)vc);
    ix = new IndexBuffer(4,ixa);

    va = new VertexArray();
    va->attachAttribute(0,cpts);

    /* This call defines the patch primitive size; I am using 4 vertices to define patches, but
       you'll need 16 */

    glPatchParameteri(GL_PATCH_VERTICES,4);

    /* mn, mx are corners of the bounding box (minimum and maximum coordinates);
       You can use min and max of the patch control vertices as the starting point to get good
       values. The teapot demo I showed in class uses mn=(-4,-4,0) and mx=(4,4,3)
    */
    mn = vec3(-1,-1,-1);
    mx = vec3(1,1,1);
    center = 0.5f*(mn+mx);
    maxdim = 2;

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);

    /* the menu allows the user to change the tesselation levels.
       64 is the highest level supported by all current GPUs I have seen 
       (and minimum required by OpenGL specification)
    */
    beginMenu();
    beginSubMenu("Inner");
    addMenuEntry("1",seti1);
    addMenuEntry("2",seti2);
    addMenuEntry("4",seti4);
    addMenuEntry("8",seti8);
    addMenuEntry("16",seti16);
    addMenuEntry("32",seti32);
    addMenuEntry("64",seti64);
    endSubMenu();
    beginSubMenu("Outer");
    addMenuEntry("1",seto1);
    addMenuEntry("2",seto2);
    addMenuEntry("4",seto4);
    addMenuEntry("8",seto8);
    addMenuEntry("16",seto16);
    addMenuEntry("32",seto32);
    addMenuEntry("64",seto64);
    endSubMenu();
    endMenu();
  }

  /* -------------------------------------- */

  void draw()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    p->setUniform("MV",
		  translate(mat4(),vec3(0.0f,0.0f,-20.0f)) *
		  mat4(getRotation()) *
		  scale(mat4(),vec3(1/maxdim)) *
		  translate(mat4(),vec3(-center[0],-center[1],-center[2]))
		  );
    p->setUniform("P",perspective(getZoom(),getAspectRatio(),18.0f,22.0f));
    p->setUniform("lloc",vec3(20.0f,20.0f,0.0f));
    p->setUniform("I",0.8f);
    p->setUniform("AI",0.2f);
    p->setUniform("ResInner",(float)rinner);
    p->setUniform("ResOuter",(float)router);
    p->setUniform("Kd",0.6f);
    p->setUniform("Ks",0.4f);
    p->setUniform("nspec",200.0f);

    p->on();

    /* the only new element is here: GL_PATCHES means that the vertex stream is broken into
       disjoint sequences of length = path size (here:4, in your code: 16). Patch is
       an abstract primitive with an arbitrary number of vertices. The geometric meaning of a patch
       is provided by tesselation shaders. Pathes are meant to be used only if the program contains
       a tesselation evaluation shader.
    */
    va->sendToPipelineIndexed(GL_PATCHES,ix,0,4);
    p->off();

    glFlush();
  }
};

int ViewerEventHandlers::rinner = 64;
int ViewerEventHandlers::router = 64;

/* -------------------------------------- */

int main ( int argc, char *argv[] )
{
  GLUTwrapper(new ViewerEventHandlers(argc,argv)).run();
  return 0;
}
