#define _USE_MATH_DEFINES
#pragma once
#include <iostream>
#include<array>
#include"MyMath.h"


struct camera_set {
	std::array<float, 4>  position = { 0 };
	std::array<float, 4>  lookat = { 0 };
	std::array<float, 4>  up = { 0 };
};
struct viewing_frustum_Prespective {
	float mynear = 0 ;
	float myfar = 0;
	float aspect = 0;//宽高比
	float fov = 0;

};

void MVP_trans(struct camera_set camera, struct myRender_vector_4& vector, struct viewing_frustum_Prespective myFrustum, int width, int height);

void Resterization(char const* filename, myRender_vector_4* vector, myRender_triangle* triangle, int triangleCount, int comp, int width, int height);

unsigned char clamp(float x);
/*void test_mvp_transform(struct camera_set camera,
	struct myRender_vector_4& original,
	struct viewing_frustum_Prespective myFrustum,  // 注意这里
	int width, int height);
struct myRender_vector_4 normalize(struct myRender_vector_4 v);*/