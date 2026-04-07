#include <iostream>
#include <cmath>
#include<array>
#include"MyMath.h"
#include"function.h"
#include"stb_image_write.h"
#define STBI_WINDOWS_UTF8
#define _USE_MATH_DEFINES

void MVP_trans(struct camera_set camera, struct myRender_vector_4& vector, struct viewing_frustum_Prespective myFrustum,int width,int height)
{
	struct Matrix3D view_matrix;

	struct Matrix3D translation;
	struct Matrix3D rotate;
	struct Matrix3D prespective;

	struct myRender_vector_4 camera_X_axis;

	
	float screen_x=0, screen_y=0;

	camera_X_axis =	cross_product3D(camera.lookat, camera.up);

	translation.matrix[0][3] = camera.position[0];
	translation.matrix[1][3] = camera.position[1];
	translation.matrix[2][3] = camera.position[2];
	translation.matrix[3][3] = 1;

	
	rotate.matrix[0][0] = camera_X_axis.position[0];
	rotate.matrix[0][1] = camera_X_axis.position[1];
	rotate.matrix[0][2] = camera_X_axis.position[2];
	rotate.matrix[0][3] = 0;

	rotate.matrix[1][0] = camera.up[0];
	rotate.matrix[1][1] = camera.up[1];
	rotate.matrix[1][2] = camera.up[2];
	rotate.matrix[1][3] = 0;

	rotate.matrix[2][0] = camera.lookat[0];
	rotate.matrix[2][1] = camera.lookat[1];
	rotate.matrix[2][2] = camera.lookat[2];
	rotate.matrix[2][3] = 0;

	rotate.matrix[3][3] = 1;

	view_matrix = Matrix3D_muilti(translation, rotate);

	float rad = myFrustum.fov * 3.1415926535f / 180.0f;
	float tan_half = tanf(rad / 2.0f);

	prespective.matrix[0][0] = 1.0f / (myFrustum.aspect * tan_half);
	prespective.matrix[1][1] = 1.0f / tan_half;
	prespective.matrix[2][2] = -(myFrustum.myfar + myFrustum.mynear) / (myFrustum.myfar - myFrustum.mynear);
	prespective.matrix[2][3] = -(2.0f * myFrustum.myfar * myFrustum.mynear) / (myFrustum.myfar - myFrustum.mynear);
	prespective.matrix[3][2] = -1.0f;

	vector = Matrix3D_muilti(view_matrix, vector);
	vector = Matrix3D_muilti(prespective, vector);
	
	float w = 1.0f / vector.position[3];
	vector.position[0] *= w;
	vector.position[1] *= w;
	vector.position[2] *= w;
	
	vector.position[0] = (vector.position[0] + 1) * 0.5 * width;
	vector.position[1] = ((1 - (vector.position[1] + 1)) * 0.5) * height;

}

void Resterization(char const* filename, myRender_vector_4& vector, int width, int height)
{


}