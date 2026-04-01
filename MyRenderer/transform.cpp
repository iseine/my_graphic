#include <iostream>
#include <cmath>
#include<array>
#include"MyMath.h"
#include"transform.h"
#define _USE_MATH_DEFINES

struct Matrix3D MVP_trans(struct camera_set camera, struct myRender_vector_4 *vector,int size)
{
	struct Matrix3D view_matrix;

	struct Matrix3D translation;
	struct Matrix3D rotate;

	struct myRender_vector_4 camera_X_axis;
	
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


}