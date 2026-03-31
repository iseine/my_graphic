#pragma once
#include <iostream>
#include<array>
#include"MyMath.h"
#define _USE_MATH_DEFINES

struct camera_set {
	std::array<float, 4>  position = { 0 };
	std::array<float, 4>  lookat = { 0 };
	std::array<float, 4>  up = { 0 };
};

struct Matrix3D MVP_trans(struct camera_set camera,myRender_vector_4 *vector,int size);