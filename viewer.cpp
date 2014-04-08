
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

    const GLfloat vc[127*3] = {
		0.2000,  0.0000, 2.70000 ,   0.2000, -0.1120, 2.70000 ,
		0.1120, -0.2000, 2.70000 ,   0.0000, -0.2000, 2.70000 ,
		1.3375,  0.0000, 2.53125 ,   1.3375, -0.7490, 2.53125 ,
		0.7490, -1.3375, 2.53125 ,   0.0000, -1.3375, 2.53125 ,
		1.4375,  0.0000, 2.53125 ,   1.4375, -0.8050, 2.53125 ,
		0.8050, -1.4375, 2.53125 ,   0.0000, -1.4375, 2.53125 ,
		1.5000,  0.0000, 2.40000 ,   1.5000, -0.8400, 2.40000 ,
		0.8400, -1.5000, 2.40000 ,   0.0000, -1.5000, 2.40000 ,
		1.7500,  0.0000, 1.87500 ,   1.7500, -0.9800, 1.87500 ,
		0.9800, -1.7500, 1.87500 ,   0.0000, -1.7500, 1.87500 ,
		2.0000,  0.0000, 1.35000 ,   2.0000, -1.1200, 1.35000 ,
		1.1200, -2.0000, 1.35000 ,   0.0000, -2.0000, 1.35000 ,
		2.0000,  0.0000, 0.90000 ,   2.0000, -1.1200, 0.90000 ,
		1.1200, -2.0000, 0.90000 ,   0.0000, -2.0000, 0.90000 ,
		-2.0000,  0.0000, 0.90000 ,   2.0000,  0.0000, 0.45000 ,
		2.0000, -1.1200, 0.45000 ,   1.1200, -2.0000, 0.45000 ,
		0.0000, -2.0000, 0.45000 ,   1.5000,  0.0000, 0.22500 ,
		1.5000, -0.8400, 0.22500 ,   0.8400, -1.5000, 0.22500 ,
		0.0000, -1.5000, 0.22500 ,   1.5000,  0.0000, 0.15000 ,
		1.5000, -0.8400, 0.15000 ,   0.8400, -1.5000, 0.15000 ,
		0.0000, -1.5000, 0.15000 ,  -1.6000,  0.0000, 2.02500 ,
		-1.6000, -0.3000, 2.02500 ,  -1.5000, -0.3000, 2.25000 ,
		-1.5000,  0.0000, 2.25000 ,  -2.3000,  0.0000, 2.02500 ,
		-2.3000, -0.3000, 2.02500 ,  -2.5000, -0.3000, 2.25000 ,
		-2.5000,  0.0000, 2.25000 ,  -2.7000,  0.0000, 2.02500 ,
		-2.7000, -0.3000, 2.02500 ,  -3.0000, -0.3000, 2.25000 ,
		-3.0000,  0.0000, 2.25000 ,  -2.7000,  0.0000, 1.80000 ,
		-2.7000, -0.3000, 1.80000 ,  -3.0000, -0.3000, 1.80000 ,
		-3.0000,  0.0000, 1.80000 ,  -2.7000,  0.0000, 1.57500 ,
		-2.7000, -0.3000, 1.57500 ,  -3.0000, -0.3000, 1.35000 ,
		-3.0000,  0.0000, 1.35000 ,  -2.5000,  0.0000, 1.12500 ,
		-2.5000, -0.3000, 1.12500 ,  -2.6500, -0.3000, 0.93750 ,
		-2.6500,  0.0000, 0.93750 ,  -2.0000, -0.3000, 0.90000 ,
		-1.9000, -0.3000, 0.60000 ,  -1.9000,  0.0000, 0.60000 ,
		1.7000,  0.0000, 1.42500 ,   1.7000, -0.6600, 1.42500 ,
		1.7000, -0.6600, 0.60000 ,   1.7000,  0.0000, 0.60000 ,
		2.6000,  0.0000, 1.42500 ,   2.6000, -0.6600, 1.42500 ,
		3.1000, -0.6600, 0.82500 ,   3.1000,  0.0000, 0.82500 ,
		2.3000,  0.0000, 2.10000 ,   2.3000, -0.2500, 2.10000 ,
		2.4000, -0.2500, 2.02500 ,   2.4000,  0.0000, 2.02500 ,
		2.7000,  0.0000, 2.40000 ,   2.7000, -0.2500, 2.40000 ,
		3.3000, -0.2500, 2.40000 ,   3.3000,  0.0000, 2.40000 ,
		2.8000,  0.0000, 2.47500 ,   2.8000, -0.2500, 2.47500 ,
		3.5250, -0.2500, 2.49375 ,   3.5250,  0.0000, 2.49375 ,
		2.9000,  0.0000, 2.47500 ,   2.9000, -0.1500, 2.47500 ,
		3.4500, -0.1500, 2.51250 ,   3.4500,  0.0000, 2.51250 ,
		2.8000,  0.0000, 2.40000 ,   2.8000, -0.1500, 2.40000 ,
		3.2000, -0.1500, 2.40000 ,   3.2000,  0.0000, 2.40000 ,
		0.0000,  0.0000, 3.15000 ,   0.8000,  0.0000, 3.15000 ,
		0.8000, -0.4500, 3.15000 ,   0.4500, -0.8000, 3.15000 ,
		0.0000, -0.8000, 3.15000 ,   0.0000,  0.0000, 2.85000 ,
		1.4000,  0.0000, 2.40000 ,   1.4000, -0.7840, 2.40000 ,
		0.7840, -1.4000, 2.40000 ,   0.0000, -1.4000, 2.40000 ,
		0.4000,  0.0000, 2.55000 ,   0.4000, -0.2240, 2.55000 ,
		0.2240, -0.4000, 2.55000 ,   0.0000, -0.4000, 2.55000 ,
		1.3000,  0.0000, 2.55000 ,   1.3000, -0.7280, 2.55000 ,
		0.7280, -1.3000, 2.55000 ,   0.0000, -1.3000, 2.55000 ,
		1.3000,  0.0000, 2.40000 ,   1.3000, -0.7280, 2.40000 ,
		0.7280, -1.3000, 2.40000 ,   0.0000, -1.3000, 2.40000 ,

		0.0000,  0.0000, 0.00000 ,   1.4250, -0.7980, 0.00000 ,
		1.5000,  0.0000, 0.07500 ,   1.4250,  0.0000, 0.00000 ,
		0.7980, -1.4250, 0.00000 ,   0.0000, -1.5000, 0.07500 ,
		0.0000, -1.4250, 0.00000 ,   1.5000, -0.8400, 0.07500 ,
		0.8400, -1.5000, 0.07500
    };

    /* ... and the patch indices here */

    const GLuint ixa[10*16] = {
		102, 103, 104, 105,   4,   5,   6,   7,
		8,   9,  10,  11,  12,  13,  14,  15 ,
		12,  13,  14,  15,  16,  17,  18,  19,
		20,  21,  22,  23,  24,  25,  26,  27,
		24,  25,  26,  27,  29,  30,  31,  32,
		33,  34,  35,  36,  37,  38,  39,  40 ,
		96,  96,  96,  96,  97,  98,  99, 100,
		101, 101, 101, 101,   0,   1,   2,   3 ,
		0,   1,   2,   3, 106, 107, 108, 109,
		110, 111, 112, 113, 114, 115, 116, 117 ,
		41,  42,  43,  44,  45,  46,  47,  48,
		49,  50,  51,  52,  53,  54,  55,  56,
		53,  54,  55,  56,  57,  58,  59,  60,
		61,  62,  63,  64,  28,  65,  66,  67 ,
		68,  69,  70,  71,  72,  73,  74,  75,
		76,  77,  78,  79,  80,  81,  82,  83 ,
		80,  81,  82,  83,  84,  85,  86,  87,
		88,  89,  90,  91,  92,  93,  94,  95,
		118, 118, 118, 118, 124, 122, 119, 121,
		123, 126, 125, 120,  40,  39,  38,  37
    };

    /* usual buffer, index buffer and vertex array construction;
       you should be able to use the same code with altered sizes */

    cpts = new Buffer(127,(vec3*)vc);
    ix = new IndexBuffer(10*16,ixa);

    va = new VertexArray();
    va->attachAttribute(0,cpts);

    /* This call defines the patch primitive size; I am using 4 vertices to define patches, but
       you'll need 16 */

    glPatchParameteri(GL_PATCH_VERTICES,16);

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
    va->sendToPipelineIndexed(GL_PATCHES,ix,0,10*16);
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
