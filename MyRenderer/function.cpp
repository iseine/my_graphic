#define STB_IMAGE_WRITE_IMPLEMENTATION
#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include<array>
#include<vector>
#include"MyMath.h"
#include"function.h"
#include"stb_image_write.h"
struct myRender_vector_4 normalize(struct myRender_vector_4 v) {
	struct myRender_vector_4 result;
	float len = sqrtf(v.position[0] * v.position[0] +
		v.position[1] * v.position[1] +
		v.position[2] * v.position[2]);
	if (len > 0.0001f) {
		result.position[0] = v.position[0] / len;
		result.position[1] = v.position[1] / len;
		result.position[2] = v.position[2] / len;
	}
	else {
		result.position[0] = 0;
		result.position[1] = 0;
		result.position[2] = 0;
	}
	result.position[3] = 0;  // 方向向量的 w=0
	result.color = v.color;   // 保留颜色
	return result;
}


unsigned char clamp(float x) {
	if (x > 255.0f) return 255;
	if (x < 0.0f) return 0;
	return (unsigned char)x;
}

void MVP_trans(struct camera_set camera, struct myRender_vector_4& vector, struct viewing_frustum_Prespective myFrustum, int width, int height)
{
	//printf("before MVP cube in MVP: (%f, %f, %f)\n", vector.position[0], vector.position[1], vector.position[2]);
	struct Matrix3D view_matrix;

	struct Matrix3D translation;
	struct Matrix3D rotate;
	struct Matrix3D prespective;

	struct myRender_vector_4 camera_X_axis;


	float screen_x = 0, screen_y = 0;

	translation.matrix[0][3] = -camera.position[0];
	translation.matrix[1][3] = -camera.position[1];
	translation.matrix[2][3] = -camera.position[2];
	translation.matrix[3][3] = 1;
	translation.matrix[0][0] = 1;
	translation.matrix[1][1] = 1;
	translation.matrix[2][2] = 1;

	myRender_vector_4 forward;
	forward.position[0] = camera.lookat[0] - camera.position[0];
	forward.position[1] = camera.lookat[1] - camera.position[1];
	forward.position[2] = camera.lookat[2] - camera.position[2];
	forward.position[3] = 0;;

	forward = normalize(forward);  // 假设 lookat 已经是方向
	struct myRender_vector_4 right = normalize(cross_product3D(forward.position, camera.up));
	struct myRender_vector_4 up = normalize(cross_product3D(right.position, forward.position));

	// 2. 构造旋转矩阵（列向量：每一列是一个轴）
	rotate.matrix[0][0] = right.position[0];
	rotate.matrix[0][1] = right.position[1];
	rotate.matrix[0][2] = right.position[2];

	rotate.matrix[1][0] = up.position[0];
	rotate.matrix[1][1] = up.position[1];
	rotate.matrix[1][2] = up.position[2];

	rotate.matrix[2][0] = -forward.position[0];
	rotate.matrix[2][1] = -forward.position[1];
	rotate.matrix[2][2] = -forward.position[2];

	rotate.matrix[3][3] = 1;

	/*rotate.matrix[1][0] = camera.up[0];
	rotate.matrix[1][1] = camera.up[1];
	rotate.matrix[1][2] = camera.up[2];
	rotate.matrix[1][3] = 0;

	rotate.matrix[2][0] = camera.lookat[0];
	rotate.matrix[2][1] = camera.lookat[1];
	rotate.matrix[2][2] = camera.lookat[2];
	rotate.matrix[2][3] = 0;

	rotate.matrix[3][3] = 1;*/

	view_matrix = Matrix3D_muilti(rotate,translation);;

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
	//printf("before div: w=%f\n", vector.position[3]);
	vector.position[0] *= w;
	vector.position[1] *= w;
	vector.position[2] *= w;

	vector.position[0] = (vector.position[0] + 1) * 0.5 * width;
	vector.position[1] = (1.0f - (vector.position[1] + 1.0f) * 0.5f) * height;
	//printf("after MVP cube in MVP: (%f, %f, %f)\n",vector.position[0], vector.position[1], vector.position[2]);

}

void Resterization(char const* filename, myRender_vector_4* vector, myRender_triangle* triangle, int triangleCount,int comp,int width, int height)
{
	//printf("in Resterization: v0=(%f,%f)\n", vector[0].position[0], vector[0].position[1]);
	//printf("tri0 indices: %d, %d, %d\n", triangle[0].index[0], triangle[0].index[1], triangle[0].index[2]);
	/*printf("tri %d: (%f,%f) (%f,%f) (%f,%f)\n",
		vector[triangle[0].index[0]].position[0], vector[triangle[0].index[0]].position[1],
		vector[triangle[0].index[1]].position[0], vector[triangle[0].index[1]].position[1],
		vector[triangle[0].index[2]].position[0], vector[triangle[0].index[2]].position[1]);*/
	int full_dot = width * height * comp;
	std::vector<unsigned char> image_xy(full_dot, 255);
	for (int i = 0; i < triangleCount; i++)
	{

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
				
				if (denominator_c == 0) { continue; }
				float c = numerator_c / denominator_c;

				float denominator_b = ((ya - yc) * xb) + ((xc - xa) * ya) + ((xa * yc) - (xc * ya));
				float numerator_b = ((ya - yc) * current_x) + ((xc - xa) * current_y) + ((xa * yc) - (xc * ya));

				if (denominator_b == 0) { continue; }
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

					if (current_x >= 0 && current_x < width &&
						current_y >= 0 && current_y < height)
					{
						int index_image = (current_y * width + current_x) * 3;
						image_xy[index_image] = r;
						image_xy[index_image + 1] = g;
						image_xy[index_image + 2] = b;
					}
				}
			
			}
		}

	}
	stbi_write_png(filename, width, height, comp, image_xy.data(), width * 3);

}
/*void test_mvp_transform(struct camera_set camera,
	struct myRender_vector_4& original,
	struct viewing_frustum_Prespective myFrustum,  // 注意这里
	int width, int height)
{
	printf("\n========== MVP TEST ==========\n");
	printf("Original vertex: (%.3f, %.3f, %.3f, %.3f)\n",
		original.position[0], original.position[1],
		original.position[2], original.position[3]);

	// 1. 计算相机坐标系轴
	struct myRender_vector_4 direction;
	direction.position[0] = camera.lookat[0] - camera.position[0];
	direction.position[1] = camera.lookat[1] - camera.position[1];
	direction.position[2] = camera.lookat[2] - camera.position[2];
	direction.position[3] = 0;
	printf("Direction (lookat - pos): (%.3f, %.3f, %.3f)\n",
		direction.position[0], direction.position[1], direction.position[2]);

	struct myRender_vector_4 camera_X_axis = cross_product3D(direction.position, camera.up);
	printf("camera_X_axis (cross(direction, up)): (%.3f, %.3f, %.3f)\n",
		camera_X_axis.position[0], camera_X_axis.position[1], camera_X_axis.position[2]);

	// 2. 打印平移矩阵
	struct Matrix3D translation = { 0 };
	translation.matrix[0][3] = -camera.position[0];
	translation.matrix[1][3] = -camera.position[1];
	translation.matrix[2][3] = -camera.position[2];
	translation.matrix[3][3] = 1;
	translation.matrix[0][0] = 1;
	translation.matrix[1][1] = 1;
	translation.matrix[2][2] = 1;
	printf("Translation: T = (%.3f, %.3f, %.3f)\n",
		translation.matrix[0][3], translation.matrix[1][3], translation.matrix[2][3]);

	// 3. 打印旋转矩阵（只打第一行，第二行，第三行）
	myRender_vector_4 lookat;
	lookat.position = camera.lookat;
	struct Matrix3D rotate = { 0 };
	struct myRender_vector_4 forward = normalize(lookat);  // 假设 lookat 已经是方向
	struct myRender_vector_4 right = normalize(cross_product3D(forward.position, camera.up));
	struct myRender_vector_4 up = normalize(cross_product3D(right.position, forward.position));

	// 2. 构造旋转矩阵（列向量：每一列是一个轴）
	rotate.matrix[0][0] = right.position[0];
	rotate.matrix[1][0] = right.position[1];
	rotate.matrix[2][0] = right.position[2];
	rotate.matrix[0][1] = up.position[0];
	rotate.matrix[1][1] = up.position[1];
	rotate.matrix[2][1] = up.position[2];
	rotate.matrix[0][2] = forward.position[0];
	rotate.matrix[1][2] = forward.position[1];
	rotate.matrix[2][2] = forward.position[2];
	rotate.matrix[3][3] = 1;
	rotate.matrix[0][0] = camera_X_axis.position[0];
	rotate.matrix[0][1] = camera_X_axis.position[1];
	rotate.matrix[0][2] = camera_X_axis.position[2];
	rotate.matrix[0][3] = 0;

	printf("Rotate matrix row0: (%.3f, %.3f, %.3f, %.3f)\n",
		rotate.matrix[0][0], rotate.matrix[0][1], rotate.matrix[0][2], rotate.matrix[0][3]);
	printf("Rotate matrix row1: (%.3f, %.3f, %.3f, %.3f)\n",
		rotate.matrix[1][0], rotate.matrix[1][1], rotate.matrix[1][2], rotate.matrix[1][3]);
	printf("Rotate matrix row2: (%.3f, %.3f, %.3f, %.3f)\n",
		rotate.matrix[2][0], rotate.matrix[2][1], rotate.matrix[2][2], rotate.matrix[2][3]);

	// 4. 计算 view_matrix = translation * rotate
	struct Matrix3D view_matrix = Matrix3D_muilti(translation, rotate);
	printf("View matrix row0: (%.3f, %.3f, %.3f, %.3f)\n",
		view_matrix.matrix[0][0], view_matrix.matrix[0][1], view_matrix.matrix[0][2], view_matrix.matrix[0][3]);

	// 5. 将顶点变换到相机空间
	struct myRender_vector_4 view_space = Matrix3D_muilti(view_matrix, original);
	printf("View space vertex: (%.3f, %.3f, %.3f, %.3f)\n",
		view_space.position[0], view_space.position[1], view_space.position[2], view_space.position[3]);

	// 6. 打印透视矩阵参数
	float rad = myFrustum.fov * 3.1415926535f / 180.0f;
	float tan_half = tanf(rad / 2.0f);
	printf("Perspective: fov=%.1f, near=%.2f, far=%.2f, aspect=%.2f\n",
		myFrustum.fov, myFrustum.mynear, myFrustum.myfar, myFrustum.aspect);
	printf("tan_half = %.6f, 1/(aspect*tan_half)=%.6f, 1/tan_half=%.6f\n",
		tan_half, 1.0f / (myFrustum.aspect * tan_half), 1.0f / tan_half);

	// 7. 打印透视矩阵（只打关键元素）
	struct Matrix3D persp = { 0 };
	persp.matrix[0][0] = 1.0f / (myFrustum.aspect * tan_half);
	persp.matrix[1][1] = 1.0f / tan_half;
	persp.matrix[2][2] = -(myFrustum.myfar + myFrustum.mynear) / (myFrustum.myfar - myFrustum.mynear);
	persp.matrix[2][3] = -(2.0f * myFrustum.myfar * myFrustum.mynear) / (myFrustum.myfar - myFrustum.mynear);
	persp.matrix[3][2] = -1.0f;
	printf("Persp matrix[2][2]=%.3f, [2][3]=%.3f, [3][2]=%.3f\n",
		persp.matrix[2][2], persp.matrix[2][3], persp.matrix[3][2]);

	// 8. 投影到 clip space
	struct myRender_vector_4 clip_space = Matrix3D_muilti(persp, view_space);
	printf("Clip space vertex: (%.3f, %.3f, %.3f, %.3f)\n",
		clip_space.position[0], clip_space.position[1], clip_space.position[2], clip_space.position[3]);


	printf("========== END ==========\n\n");
}
*/