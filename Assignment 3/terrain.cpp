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
    for(int i =0; i<iterations; i++){
        
        float v = static_cast <float> (rand());
        float a = sin(v);                                //x co-effcient 
        float b = cos(v);                                //y co-effcient 
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
/* circles algorithm method for creating a height map for the terrain*/
void terrain::circlesAlgorithm (int iterations){
    //intialize min and max height values                           //increase circle height by 3
    for(int i =0; i<iterations; i++){
        int random_x = rand()%size_x;                         //randomly select x center point for circle
        int random_z = rand()%size_z;                         //randomly select z center point for circle
        int random_radius = (rand()%20)+1;                   //randomly generate radius integer between 1 - 20
        float disp = (rand()%12)+1;
        for(int x = 0; x<size_x; x++){
            for(int z= 0; z<size_z; z++){
                int dx = x - random_x;
                int dz = z - random_z;
                float distance = sqrtf((dx*dx) + (dz*dz));
                float pd = (distance*2)/random_radius;
                if (fabs(pd) <= 1.0){
                    heightMap[x][z] += (disp/2.0) + (cos(pd*3.14)*(disp/2.0));
                }
            }
        }
        
    }
    //find new max and min height values of the entire map
    minHeight = 0;
    maxHeight = 0;
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
    for(int x =1; x<size_x-1; x++){
        for(int z=1; z<size_z-1; z++){
            glBegin(GL_QUADS);
            for(int i = 0; i<4; i++){
                //draw polygon for each point square of the terrain
                int new_x = x-quadArray[i][0];
                int new_z  =z-quadArray[i][1];
                float new_y = heightMap[new_x][new_z];
                
                if(lightingMode == OFF){
                    float useColor = (new_y - minHeight)/(maxHeight-minHeight);
                    glColor3f(useColor, useColor, useColor);
                }
                else{
                    //if colour mode selected add colour to terrain depending on height
                    if(colorMode == ON){
                        float height_r = (new_y - minHeight)/(maxHeight-minHeight);
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
                        glColor3fv(twitterColor);
                    }
                }
                //normal vectors for each polygon
                glNormal3fv(normalVectors[new_x][new_z]);
                glVertex3d(new_x, new_y, new_z);
            }
            glEnd();
        }
    }
}
/* calculate normal vectors for each polygon of the terrain */
void terrain::buildNormalVectors(){
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

