//
//  terrain.h
//  Assignment 3
//
//  Created by Simar Singh on 2014-11-26.
//  Copyright (c) 2014 Simarpreet Singh. All rights reserved.
//

#ifndef __Assignment_3__terrain__
#define __Assignment_3__terrain__

#include <stdio.h>

//using namespace std;

class terrain{
public:
    //declare constants for quick coding later
    const int OFF = 0;       
    const int ON = 1;
    const int DEFAULT = 2;
    const int CIRCLE = 3;
    
    //Colors rgb values 
    float whiteColor[3] = {1.0, 1.0, 1.0};
    float twitterColor[3] = {0.33, 0.675, 0.93};
    float waterColor[3] = {0, 0, 0.9};
    float grassColor[3] = {0, 0.6, 0};
    float dirtColor[3] = {0.6, 0.4, 0.2};
    
    //unit polygon to create quads
    //clockwise spin from bottom left, top left, top right, bottom right
    int quadArray[4][2] = {{0,0}, {0,-1}, {-1,-1}, {-1,0}};
    
    float heightMap[1000][1000]; //declare heightmap and its maximum size possible of 1000 x 1000
    float normalVectors[1000][1000][3]; //declare the normal vector array of the same max size of 1000 x 1000
    //size of terrain x, z values
    int size_x;
    int size_z;
    //maximum and minimum height values
    float minHeight, maxHeight;
    int terrainType; //terrainType is an int as we have declared 2 = DEFAULT and 3 = CIRCLE above
    
    //constructors
    terrain();
    terrain(int size_x, int size_z, int type, int iterations);
    terrain(int size_x, int size_z);
    
    //methods
    void faultAlgorithm(int iterations);      //fault algorithm for height map
    void circlesAlgorithm (int iterations);     //circles algorithm for height map
    void buildNormalVectors();      //construct normal vectors for the height map
    
    void drawTerrain(int lightingMode, int colorMode); //draw the terrain to the screen
    float * getColorByHeight(float height, int lightingMode, int colorMode);  //not used
    
    
};

#endif /* defined(__Assignment_3__terrain__) */
