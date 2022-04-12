//PURELY DEMO FILLED FILE
//Main file for absolute freedom, all draw programming not opengl-core specifc should be done in here.
//Standard libraries
#define FREEGLUT_STATIC
#define _LIB
#define FREEGLUT_LIB_PRAGMAS 0
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <GL/glut.h>
//External engine dependencies
#include "vectorlib.hpp"
#include "cameralib.hpp"
#include "interfacelib.hpp"
#include "shaderslib.hpp"
#include "lightinglib.hpp"
//CROSS-FILE VALUES: Freely change these
char VERTEXSHADERFILE[BUFSIZE] = "samplevs.vert";
char FRAGMENTSHADERFILE[BUFSIZE] = "samplefg.frag";
char WINDOWTITLE[BUFSIZE] = "EAGLE";
unsigned int DISPLAYFLAGS = GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA | GLUT_ALPHA | GLUT_MULTISAMPLE;
unsigned int ENABLEFLAGS = GL_DEPTH_TEST;

object3d terrain;
object3d surface;
object3d buildings[50];
object3d initialb[10];
object3d signholder;
object3d treeholder;
object3d lampholder;
sceneobj mainscene;
//Sample variables
GLuint renderlist;
GLuint signtexture;
GLuint maintexture;
GLuint groundtexture;
GLuint buildtextures[5];

std::vector<object3d> signs(200);
std::vector<object3d> trees(200);
std::vector<object3d> lamps(200);
object3d car;
//Sample demo glsetup, utilizes lighting, texturing, and a legacy display list
void glsetup (void) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    car.importmesh3d("mercedes.obj");
    car.scaleobj3d(400);
    treeholder.importmesh3d("tree.obj");
    signholder.importmesh3d("sign.obj");
    lampholder.importmesh3d("streetlamp.obj");
    for (int n2 = 0; n2 < 5; n2++) {
        for (int n = 0; n <= 9; n++) {
            if (n2 == 0) {
                std::string tmp = "Buildings//Residential Buildings 00";
                tmp += std::to_string(n);
                tmp += ".obj";
                std::cout << "Loading in " << tmp << "..." << std::endl;
                initialb[n].importmesh3d(tmp.c_str());
                initialb[n].texturetransformfactor = 0.01;
            }
            buildings[n2 * 9 + n] = initialb[n];
            buildings[n2 * 9 + n].translateobj3f(rand() % 300 + (-150), 0, rand() % 300 + (-150));
            buildings[n2 * 9 + n].scaleobj3d(200);
            
        }
    }
    for (int i = 0; i < 30; i++) {
        lamps[i].vertexbuf = lampholder.vertexbuf;
        lamps[i].uvbuf = lampholder.uvbuf;
        lamps[i].normalsbuf = lampholder.normalsbuf;
        lamps[i].texturetransformfactor = 0.0005;
        lamps[i].translateobj3f(rand() % 200 + (-100), 0, rand() % 200 + (-100));
        lamps[i].scaleobj3d(300);

        signs[i].vertexbuf = signholder.vertexbuf;
        signs[i].uvbuf = signholder.uvbuf;
        signs[i].normalsbuf = signholder.normalsbuf;
        signs[i].texturetransformfactor = 0.0005;
        signs[i].translateobj3f(rand() % 750 + (-375), 0, rand() % 750 + (-375));
        signs[i].scaleobj3d(100);
        /*signs[i].objcolor = color4ub(66, 189, 200);
        signs[i].defprimtype = GL_TRIANGLES;
        signs[i].objcollider.generatehitbox();
        signs[i].addasset(mainscene);*/
    }
    for (int i = 0; i < 100; i++) {
        trees[i].vertexbuf = treeholder.vertexbuf;
        trees[i].uvbuf = treeholder.uvbuf;
        trees[i].normalsbuf = treeholder.normalsbuf;
        trees[i].texturetransformfactor = 0.0005;
        trees[i].translateobj3f(rand() % 900 + (-450), 0, rand() % 900 + (-450));
        trees[i].scaleobj3d(100);
        trees[i].objcolor = color4ub(66, 189, 200);
        trees[i].defprimtype = GL_TRIANGLES;
        trees[i].objcollider.generatehitbox();
        trees[i].addasset(mainscene);
    }
    surface.importmesh3d("patch.obj");
    surface.texturetransformfactor = 0.1;
    surface.translateobj3f(0, -7.75, 0);
    surface.scaleobj3d(500);
    
    //initlightsource(vertex3d(1.0, 1.0, 1.0));
    buildtextures[0] = LoadTexture("texture02.jpg");
    buildtextures[1] = LoadTexture("texture03.jpg");
    buildtextures[2] = LoadTexture("texture02.png");
    buildtextures[3] = LoadTexture("graybrick.png");
    groundtexture = LoadTexture("grass.jpg");
    signtexture = LoadTexture("texture02.png");
    renderlist = glGenLists(1);
    
    glNewList(renderlist, GL_COMPILE);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    for (int n = 0; n < 30; n++) {
        glBindTexture(GL_TEXTURE_2D, buildtextures[n % 3]);
        buildings[n].rendermesh3d(GL_TRIANGLES);
    }
    glBindTexture(GL_TEXTURE_2D, signtexture);
    for (int i = 0; i < 50; i++) {
        signs[i].rendermesh3d(GL_TRIANGLES);
        lamps[i].rendermesh3d(GL_TRIANGLES);
    }
    glBindTexture(GL_TEXTURE_2D, groundtexture);
    surface.rendermesh3d(GL_TRIANGLES);


    glDisable(GL_TEXTURE_2D);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for (int n = 0; n < 30; n++) {
        buildings[n].rendermesh3d(100,100,100,100,GL_TRIANGLES);
    }
    surface.rendermesh3d(255, 255, 255, 255, GL_TRIANGLES);
    renderscene(mainscene);
    glEndList();
}

void bindinterface (void) {
    bindmousecontrol();
    bindarrowkeycontrol();
    bindnormalkeycontrol();
}

void eventhandler(void) {
}

void mainprocess (void) {
    glCallList(renderlist);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    car.rendermesh3d(255, 255, 255, 255, GL_TRIANGLES);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    car.rendermesh3d(100, 100, 100, 255, GL_TRIANGLES);
    glFlush();
}