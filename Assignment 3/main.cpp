//
//  main.cpp
//  Assignment 3
//
//  Created by Simarpreet Singh and Birunthaa Umamahesan on 2014-11-25.
//  Copyright (c) 2014 Simarpreet Singh and Birunthaa Umamahesan. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <iostream>
#include <math.h>
#include "terrain.h"

using namespace std;
/* GLOBAL VARIABLES */

//mode on or off constants
const int OFF = 0;
const int ON = 1;

// default algorithm for terrain constant selection option
const int DEFAULT = 2;

// circle algorithm for terrain constant selection option
const int CIRCLE = 3;

// two different light position intialization values
float light_pos[] = {150,-1000, 150,1};
float light_pos2[] = {-150,-1000, -150,1};

float pos[] = {0,1,0};

float camPos[] = {80, 130, 80};
float angle = 0.0f;

// terrain size variables
int terrainSizeX = 100;
int terrainSizeZ = 100;
int terrainIterations = ((terrainSizeX+terrainSizeZ)/2)*3;

//intial mode options state
int wireFrameMode = ON;
int cirlceMode = OFF;
int lightingMode = ON;
int colorMode = OFF;
int flatShadingMode = OFF;
int flipTerrainMode = OFF;

//intialize terrain object
terrain * mainTerrain = new terrain(terrainSizeX, terrainSizeZ, DEFAULT, terrainIterations);

//remake the terrain using the constructor depending on the mode
void remakeTerrain(){
    if (cirlceMode == ON) {
        mainTerrain = new terrain(terrainSizeX, terrainSizeZ, CIRCLE, terrainIterations);
    }
    else{
        mainTerrain = new terrain(terrainSizeX, terrainSizeZ, DEFAULT, terrainIterations);
    }
}

/* key presses control terrain appearance*/
void keyboard(unsigned char key, int x, int y){
    switch (key)
    {
            //if 'q' pressed -> quit program
        case 'q':
        case 27:
            exit (0);
            break;
            //if 'w' -> wireframe mode otherwise normal filled shape
        case 'w':
        case 'W':
            if(wireFrameMode == ON){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireFrameMode = OFF;
            }
            else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireFrameMode = ON;
            }
            break;
            // if 's' -> shadding mode otherwise shadding is not applied
        case 's':
        case 'S':
            if(flatShadingMode == ON){
                flatShadingMode = OFF;
                glShadeModel(GL_FLAT);
            }
            else{
                flatShadingMode = ON;
                glShadeModel(GL_SMOOTH);
            }
            break;
            // reset terrain
        case 'r':
        case 'R':
            remakeTerrain();
            break;
            // if 'c' -> circle mode selected, use circle algorithm to generate terrain otherwise use fault
        case 'c':
        case 'C':
            if (cirlceMode == ON) {
                mainTerrain = new terrain(terrainSizeX, terrainSizeZ, DEFAULT, terrainIterations);
                cirlceMode = OFF;
            }
            else{
                mainTerrain = new terrain(terrainSizeX, terrainSizeZ, CIRCLE, terrainIterations);
                cirlceMode = ON;
            }
            break;
            //if 'l' -> lighting mode is on
        case 'l':
        case 'L':
            if (lightingMode == ON){
                lightingMode = OFF;
                glDisable(GL_LIGHT0);
                glDisable(GL_LIGHTING);
            }
            else{
                lightingMode = ON;
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHTING);
            }
            break;
            // if 't' -> Colour terrain according to topographic maps
        case 't':
        case 'T':
            if (colorMode == ON){
                colorMode = OFF;
            }
            else{
                colorMode =  ON;
            }
            break;
            
            // if 'j' -> decrease light 1 brightness
        case 'j':
        case 'J':
            light_pos[1] -= 10;
            light_pos2[1] -= 10;
            break;
            // if 'u' -> increase light 1 brightness
        case 'u':
        case 'U':
            light_pos[1] += 10;
            light_pos2[1] += 10;
            break;
            // if 'h' -> increase light 2 brightness
        case 'h':
        case 'H':
            light_pos[2] += 10;
            light_pos2[2] += 10;
            break;
            // if 'k' -> decrease light 2 brightness
        case 'K':
        case 'k':
            light_pos[2] -= 10;
            light_pos2[2] -= 10;
            break;
            
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
    glPopMatrix();
    glutPostRedisplay();
}

/* special translates the camera according to key press */
void special(int key, int x, int y){
    /* arrow key presses move the camera */
    switch(key)
    {
            //left keypress, translate left on the x axis
        case GLUT_KEY_LEFT:
            camPos[2] +=  2;
            break;
            //right keypress, translate right on the x axis
        case GLUT_KEY_RIGHT:
            camPos[2] -= 2;
            break;
            //up keypress, translate back on the z axis
        case GLUT_KEY_UP:
            if (camPos[1]<0){
                flipTerrainMode = OFF;
                camPos[0]*=-1;
                camPos[2]*=-1;
            }
            if (flipTerrainMode == ON){
                camPos[1] -= 2;
            }
            else{
                camPos[1] += 2;
            }
            break;
            //down keypress, translate up on the z axis
        case GLUT_KEY_DOWN:
            if (camPos[1]<0){
                flipTerrainMode = ON;
                camPos[0]*=-1;
                camPos[2]*=-1;
            }
            if (flipTerrainMode == OFF){
                camPos[1] -= 2;
            }
            else{
                camPos[1] += 2;
            }
            break;
            
        case GLUT_KEY_HOME:
            camPos[0] += 2;
            break;
            
        case GLUT_KEY_END:
            camPos[0] -= 2;
            break;
            
    }
    glutPostRedisplay();
}
/* menu method called at to create right click menu
 * includes different sizes of terrains
 */
void menu(int op) {
    
    switch(op) {
        case 'a':
            terrainSizeX = 50;
            terrainSizeZ = 50;
            break;
        case 'b':
            terrainSizeX = 100;
            terrainSizeZ = 100;
            break;
        case 'c':
            terrainSizeX = 150;
            terrainSizeZ = 150;
            break;
        case 'd':
            terrainSizeX = 300;
            terrainSizeZ = 300;
            break;
    }
    remakeTerrain();
    glutPostRedisplay();
}


/* display method called at rendering time
 * displays the output of the terrain mesh
 */
void display(void){
    //setup of material values for light
    float m_amb[] = {0.83, 0.52, 0.63, 1.0};
    float m_dif[] = {0.28, 0.87, 0.11, 1.0};
    float m_spec[] = {0.99, 0.91, 0.81, 1.0};
    float shiny = 27;
    
    //enable material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glColorMaterial(GL_AMBIENT, GL_DIFFUSE);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //create camera viewing transformations
    gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
    glTranslatef(-1*(terrainSizeX/2), 0, -1*(terrainSizeX/2));
    //draw terrain to screen
    mainTerrain->drawTerrain(lightingMode, colorMode);
    
    glutSwapBuffers();
}

/* method for intializing the terrain setup */
void init(void){
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos2);
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glOrtho(-2, 2, -2, 2, -2, 2);
    gluPerspective(45, 1, 1, 400);
    
    //enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    
}

/* instructions method prints out control key functions for user */
void instructions(void){
    printf("=======================================================\n");
    printf("Terrain Generator\n\nBy\nSimarpreet Singh(1216728)\n and Birunthaa Umamahesan(1203142)\n");
    printf("=======================================================\n");
    printf("\n");
    printf("           CONTROLS    \n");
    printf("+----------------------------+\n");
    printf("|  w  : Wireframe Mode       |\n");
    printf("|-----+----------------------|\n");
    printf("|  l  : Toggle Light Mode    |\n");
    printf("|-----+----------------------|\n");
    printf("|  s  : Toggle Flat Shadding |\n");
    printf("|-----+----------------------|\n");
    printf("|  r  : Random Terrain       |\n");
    printf("|-----+----------------------|\n");
    printf("|  c  : Circle Algorithm     |\n");
    printf("|-----+----------------------|\n");
    printf("|  t  : Toggle Color Mode    |\n");
    printf("+----------------------------+\n");
    printf("|u,h,j,k: Lights Control     |\n");
    printf("|-----+----------------------|\n");
    printf("|Right Click: Terrain Sizes  |\n");
    printf("|-----+----------------------|\n");
    printf("|  q  : Quit Program         |\n");
    printf("+----------------------------+\n");
    printf("\n");
    
}

int main(int argc, char ** argv){
    
    
    glutInit(&argc, argv);		//starts up GLUT
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    
    glutCreateWindow("Assignment 3: Terrain Mesh");	//creates the window
    
    glutDisplayFunc(display);	//registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    
    glEnable(GL_DEPTH_TEST);
    
    
    instructions();                 //call instructions menu
    
    init();                         //intial method
    
    /* set up a simple menu, which will be handled by menuProc */
    glutCreateMenu(menu);
    glutAddMenuEntry("50 x 50", 'a');
    glutAddMenuEntry("100 x 100", 'b');
    glutAddMenuEntry("150 x 150", 'c');
    glutAddMenuEntry("300 x 300", 'd');
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutMainLoop();				//starts the event loop
    
    return(0);					//return may not be necessary on all compilers
}


