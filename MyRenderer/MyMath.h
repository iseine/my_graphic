#pragma once

struct Matrix3D {
	float matrix[4][4] = { 0 };

};
struct Matrix2D {
	double matrix[3][3] = { 0 };
};
struct myRender_vector_4 {
	std::array<float, 4>  position = {0};
	//float position[4] = { 0 };
	std::array<short, 3> color = {0};
	//short color[3] = { 0 };

};
struct myRender_triangle {
	std::array<int, 3> index;
};

struct myRender_vector_4 cross_product3D(const std::array<float, 4>& a, const std::array<float, 4>& b);
//3D
struct Matrix3D Matrix3D_muilti(struct Matrix3D mat1, struct Matrix3D mat2);
struct myRender_vector_4 Matrix3D_muilti(struct Matrix3D mat, struct myRender_vector_4 vector);
struct Matrix3D Matrix3D_add(struct Matrix3D mat1, struct Matrix3D mat2);
