//
//  terrain.cpp
//  Assignment 3
//
//  Created by Simar Singh on 2014-11-26.
//  Copyright (c) 2014 Simarpreet Singh. All rights reserved.
//

#include "terrain.h"
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
#include "math.h"
#include <algorithm>    // std::min

/* default terrain construction */
terrain::terrain(){
    size_x = 50;
    size_z = 50;
    terrainType = DEFAULT;
    faultAlgorithm(100);
}

/* user given input terrain constructor */
terrain::terrain(int size_x_given, int size_z_given, int type_given, int iterations){
    //create the terrain from the given inputs
    size_x = size_x_given;
    size_z = size_z_given;
    terrainType = type_given;
    if(terrainType==CIRCLE){
        circlesAlgorithm(iterations);               //use circles algorithm for the terrain 
    }
    else{
        faultAlgorithm(iterations);                 //use default fault algorithm for the terrain
    }
    buildNormalVectors();                           //calculate normals for each polygon
}

/* fault algorithm method for creating a height map for the terrain*/
void terrain::faultAlgorithm(int iterations){
    //run the algorithm for a predetermined set of times doing the following operations
    for(int i =0; i<iterations; i++){
        
        float v = static_cast <float> (rand());          //create a random floating point number from {0-1}
        float a = sin(v);                                //take the sin of the number for the x co-effcient 
        float b = cos(v);                                //take the cos of the number for the y co-effcient 
        float d = sqrt(size_x*size_x + size_z*size_z);   //distance from the furthest point 
        
        //random c value smaller then the distance from the furthest point of the terrain
        float c = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * d - d/2;
       
        // for each x,z point on the map determine if increase or decrease height
        for(int x = 0; x<size_x; x++){
            for(int z=0; z<size_z; z++){
                if (a*x + b*z - c > 0){                    //increase height
                    heightMap[x][z] += 1;
                    if(heightMap[x][z]>maxHeight){
                        maxHeight = heightMap[x][z];      //calculate new max-height value
                    }
                }
                else{
                    heightMap[x][z] -= 1;                //decrease height
                    if(heightMap[x][z]<minHeight){
                        minHeight = heightMap[x][z];    //calculate new min-height value
                    }
                }
            }
        }
    }
}
/* circles algorithm method for creating a height map for the terrain
Information on Circle Algorithm for Terrain Generation can be found at
http://www.lighthouse3d.com/opengl/terrain/index.php?circles */
void terrain::circlesAlgorithm (int iterations){
    //loop through the algorithm for the given number of iterations
    for(int i =0; i<iterations; i++){
        //create a circle at an x and y with radius between 1 and 20 using random numbers
        int random_x = rand()%size_x;                         //randomly select x center point for circle
        int random_z = rand()%size_z;                         //randomly select z center point for circle
        int random_radius = (rand()%20)+1;                   //randomly generate radius integer between 1 - 20 to be the terrainCircleSize
        float disp = (rand()%12)+1;                         //randomly generate a value of disp between 1 - 12
        //loop through the entire height map and run the circle algorithm to check if a given x and y is within the circle and
        //if so apply the algorithm to it to modify the height to the new value
        for(int x = 0; x<size_x; x++){                      
            for(int z= 0; z<size_z; z++){
                int dx = x - random_x;                      //get the value for tx to determine distance from circle center
                int dz = z - random_z;                      //get the value for ty to determine distance from circle center
                float distance = sqrtf((dx*dx) + (dz*dz));  //get the distance from circle center
                float pd = (distance*2)/random_radius;      //get the value for pd
                if (fabs(pd) <= 1.0){                       //check if value is within the circle
                    heightMap[x][z] += (disp/2.0) + (cos(pd*3.14)*(disp/2.0)); //apply circle alorithm to get the new height
                }
            }
        }
        
    }
    //find new max and min height values of the entire map
    minHeight = 0;
    maxHeight = 0;
    //loop through every value and compare it to the current highest and lowest values found
    //values found that are higher or lower become the new high or low
    //operation is n^2 complexity
    for(int x=0; x<size_x; x++){                       
        for(int z=0; z<size_z; z++){
            if(heightMap[x][z]<minHeight){
                minHeight = heightMap[x][z];
            }else if(heightMap[x][z]>maxHeight){
                maxHeight = heightMap[x][z];
            }
            
        }
    }
    
}

/* draw the terrain according to the height map, given the colour mode and light mode conditions*/
void terrain::drawTerrain(int lightingMode, int colorMode){
    //we map out the entire height array and create quads that are drawn to the screen
    for(int x =1; x<size_x-1; x++){
        for(int z=1; z<size_z-1; z++){
            glBegin(GL_QUADS); //OpenGL command to begin drawing shapes, in this case quads
            for(int i = 0; i<4; i++){
                //draw polygon for each point square of the terrain
                //GL_QUADS draws quadrilaterals in a clockwise direction starting from the bottom left
                int new_x = x-quadArray[i][0];  //coordinates to draw the quads are already prefined
                int new_z  =z-quadArray[i][1];  
                float new_y = heightMap[new_x][new_z]; //height of section acquired from heightmap float array to be 
                
                if(lightingMode == OFF){
                    //we calculate a normal {0-1} for the height of the current segment to use as
                    //a black to white gradiant to show the peaks of the terrain in white
                    float useColor = (new_y - minHeight)/(maxHeight-minHeight);
                    glColor3f(useColor, useColor, useColor);
                }
                else{
                    //if colour mode selected add colour to terrain depending on height
                    if(colorMode == ON){
                        //we calculate a normal {0-1} for the height of the current segment as seen before
                        float height_r = (new_y - minHeight)/(maxHeight-minHeight);
                        //we use a different colour scheme for the different algorithms 
                        //we then assign a colour to the segment based off its height 
                        if(terrainType == CIRCLE){
                            if (height_r>=0.5){
                                glColor3f(1, 1, 1);
                            }
                            else if(height_r>=0.3){
                                glColor3fv(dirtColor);
                            }
                            else if(height_r>=0.1){
                                glColor3fv(grassColor);
                            }
                            else{
                                glColor3fv(waterColor);
                            }
                        }
                        else{
                            if (height_r>=0.7){
                                glColor3f(1, 1, 1);
                            }
                            else if(height_r>=0.6){
                                glColor3fv(dirtColor);
                            }
                            else if(height_r>=0.5){
                                glColor3fv(grassColor);
                            }
                            else{
                                glColor3fv(waterColor);
                            }
                        }
                    }
                    else{
                        glColor3fv(twitterColor); //default colour if 
                    }
                }
                //create a normal vector and a vertex out of the values
                glNormal3fv(normalVectors[new_x][new_z]);
                glVertex3d(new_x, new_y, new_z);
            }
            glEnd();
        }
    }
}
/* calculate normal vectors for each polygon of the terrain */
void terrain::buildNormalVectors(){
    //define vectors 
    float v1[3];
    float v2[3];
    float v[3];
    for(int x =0; x<size_x; x++){
        for(int z=0; z<size_z; z++){
            v1[0] = x+1;
            v1[1] = heightMap[x+1][z] - heightMap[x][z];
            v1[2] = z;
            
            v2[0] = x+1;
            v2[1] = heightMap[x+1][z+1] - heightMap[x][z];
            v2[2] = z+1;
            
            //calculate cross product
            v[0] = v1[1]*v2[2] - v1[2]*v2[1];
            v[1] = v1[2]*v2[0] - v1[0]*v2[2];
            v[2] = v1[0]*v2[1] - v1[1]*v2[0];
            float l = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            
            //normal vectors
            normalVectors[x][z][0] = v[0]/l;
            normalVectors[x][z][1] = v[1]/l;
            normalVectors[x][z][2] = v[2]/l;
        }
    }
}

