#include <iostream>
#include <cmath>
#include<array>
#include<vector>
#include"MyMath.h"
#include"function.h"
#include"stb_image_write.h"
#define STBI_WINDOWS_UTF8
#define _USE_MATH_DEFINES

unsigned char clamp(float x) {
	if (x > 255.0f) return 255;
	if (x < 0.0f) return 0;
	return (unsigned char)x;
}

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
	vector.position[1] = (1.0f - (vector.position[1] + 1.0f) * 0.5f) * height;

}

void Resterization(char const* filename, myRender_vector_4* vector, myRender_triangle* triangle, int triangleCount,int comp,int width, int height)
{
	int full_dot = width * height * comp;
	std::vector<int> image_xy(full_dot, 0);
	for (int i = 0; i < triangleCount; i++)
	{
		/*
		float A = triangle[i].index[0];//三角形顶点A
		float B = triangle[i].index[1];//三角形顶点B
		float C = triangle[i].index[2];//三角形顶点C*/

		float xmin = 10000, ymin = 10000, xmax = -10000, ymax = -10000;
		for (int j = 0; j < 3; j++)
		{
			if (vector[triangle[i].index[j]].position[0] > xmax) xmax = vector[triangle[i].index[j]].position[0];
			if (vector[triangle[i].index[j]].position[1] > ymax) ymax = vector[triangle[i].index[j]].position[1];

			if (vector[triangle[i].index[j]].position[0] < xmin) xmin = vector[triangle[i].index[j]].position[0];
			if (vector[triangle[i].index[j]].position[1] < ymin) ymin = vector[triangle[i].index[j]].position[1];
		}
		for (int current_x=xmin; current_x <= xmax; current_x++)
		{
			for (int current_y=ymin; current_y <= ymax; current_y++)
			{
				//当前像素image[current_y*(ymax-ymin)+current_x],当前点(current_x,current_y)
				float xa = vector[triangle[i].index[0]].position[0];
				float ya = vector[triangle[i].index[0]].position[1];

				float xb = vector[triangle[i].index[1]].position[0];
				float yb = vector[triangle[i].index[1]].position[1];

				float xc = vector[triangle[i].index[2]].position[0];
				float yc = vector[triangle[i].index[2]].position[1];

				float denominator_c = ((ya - yb) * xc) + ((xb - xa) * yc) + ((xa * yb) - (xb * ya));
				float numerator_c = ((ya - yb) * current_x) + ((xb - xa) * current_y) + ((xa * yb) - (xb * ya));
				
				if (denominator_c == 0) { continue; }//
				float c = numerator_c / denominator_c;

				float denominator_b = ((ya - yc) * xb) + ((xc - xa) * ya) + ((xa * yc) - (xc * ya));
				float numerator_b = ((ya - yc) * current_x) + ((xc - xa) * current_y) + ((xa * yc) - (xc * ya));

				if (denominator_b == 0) { continue; }//
				float b = numerator_b / denominator_b;

				float a = 1 - b - c;

				if (a > 0 && c > 0 && b > 0)
				{
					float color_r = (a * vector[triangle[i].index[0]].color[0]) + (b * vector[triangle[i].index[1]].color[0]) + (c * vector[triangle[i].index[2]].color[0]);
					float color_g = (a * vector[triangle[i].index[0]].color[1]) + (b * vector[triangle[i].index[1]].color[1]) + (c * vector[triangle[i].index[2]].color[1]);
					float color_b = (a * vector[triangle[i].index[0]].color[2]) + (b * vector[triangle[i].index[1]].color[2]) + (c * vector[triangle[i].index[2]].color[2]);

					unsigned char r = clamp(color_r);
					unsigned char g = clamp(color_g);
					unsigned char b = clamp(color_b);
				}
			}
		}

	}


}