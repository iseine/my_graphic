#define _USE_MATH_DEFINES
#include <iostream>
#include<array>
#include"MyMath.h"


using namespace std;

struct myRender_vector_4 cross_product3D(const std::array<float, 4>& a, const std::array<float, 4>& b)
{
	struct myRender_vector_4 sum = { 0 };
	sum.position[0] = a[1] * b[2] - a[2] * b[1];
	sum.position[1] = a[2] * b[0] - a[0] * b[2];//sum.position[1] = a[0] * b[2] - a[2] * b[0];
	sum.position[2] = a[0] * b[1] - a[1] * b[0];
	return sum;
}


struct Matrix3D Matrix3D_muilti(struct Matrix3D mat1, struct Matrix3D mat2)
{
	struct Matrix3D sum = { 0 };
	for (int i = 0; i < 4; i++)//行循环
	{
		for (int j = 0; j < 4; j++)//列循环
		{
			for (int k = 0; k < 4; k++)//累加循环
			{
				sum.matrix [i][j]+=mat1.matrix[i][k]*mat2.matrix[k][j];
			}
		}
	}
	return sum;
}

struct myRender_vector_4 Matrix3D_muilti(struct Matrix3D mat,struct myRender_vector_4 vector )
{

	struct myRender_vector_4 sum = {0};
	sum.color = vector.color;
	sum.view_z = vector.view_z;
	for (int i = 0; i <4 ; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sum.position[i] += mat.matrix[i][j] * vector.position[j];
		}
	}
	return sum;
}



struct Matrix3D Matrix3D_add(struct Matrix3D mat1, struct Matrix3D mat2)
{
	struct Matrix3D sum = { 0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sum.matrix[i][j] = mat1.matrix[i][j] + mat2.matrix[i][j];
		}
	}
	return sum;
}