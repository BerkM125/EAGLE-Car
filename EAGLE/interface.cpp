/*
* EXTERNAL INTERFACING CPP FILE
* Contains functions pertaining to and providing functionality for
* interfaces, such as keyboard and mouse. Also contains sample player movement
* and camera movement functions that can be bound to the main OpenGL process. 
*/
//Standard libraries
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
//External engine dependencies
#include "vectorlib.hpp"
#include "cameralib.hpp"
#include "interfacelib.hpp"

float velocity = 0.0f;
float acceleration = 0.04f;
//float deacceleration = 0.01f;
extern object3d car;
void mainmousecontrol(int xx, int yy) {
    //Put any mouse code and controls you want in here, the code below is simply an example
    mousecameracontrol(xx, yy);
}

void mainnormalkeycontrol(unsigned char key, int xx, int yy) {
    //Put any normal key code and controls you want in here, the code below is simply an example
    //normalkeycameracontrol(key, xx, yy);
    std::cout << "POSITION: " << camx << ", " << camy << ", " << camz << std::endl;
    if (key == 'w') { // W
        camx += (lx * movincrement) * velocity;
        camz += (lz * movincrement) * velocity;
        car.translateobj3f((lx * movincrement) * velocity, 0, (lz * movincrement) * velocity);
        if (velocity < 0) velocity = 0.0f;
        velocity += (velocity < 5.0f) ? acceleration : 0;
    }
    if (key == 's') { // S
        //car.translateobj3f(lx * movincrement, 0, lz * movincrement);
        camx += (lx * movincrement) * velocity;
        camz += (lz * movincrement) * velocity;
        car.translateobj3f((lx * movincrement) * velocity, 0, (lz * movincrement) * velocity);
        if (velocity > 0) velocity = 0.0f;
        velocity -= (velocity > -5.0f) ? acceleration*2 : 0;
    }
    if (key == 'd') { // D
        angle += angleincrement;
        lx = sin(angle);
        lz = -cos(angle);
    }
    if (key == 'a') { // A
        angle -= angleincrement;
        lx = sin(angle);
        lz = -cos(angle);
    }
    if (key == 27)
        exit(0);
}

void mainarrowkeycontrol(int key, int xx, int yy) {
    //Put any arrow key involving code and controls you want in here, the code below is simply an example
    //arrowkeycameracontrol(key, xx, yy);
    switch (key) {
    case GLUT_KEY_UP:
        camy += movincrement;
        break;
    case GLUT_KEY_DOWN:
        camy -= movincrement;
        break;
    }
}

void bindmousecontrol (void) {
    glutMotionFunc(mainmousecontrol);
}

void bindarrowkeycontrol (void) {
    glutSpecialFunc(mainarrowkeycontrol);
}

void bindnormalkeycontrol (void) {
    glutKeyboardFunc(mainnormalkeycontrol);
}