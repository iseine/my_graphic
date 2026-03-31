#include <iostream>
#include<array>
#include"MyMath.h"
#define _USE_MATH_DEFINES

using namespace std;


struct Matrix3D Matrix3D_muilti(struct Matrix3D mat1, struct Matrix3D mat2)
{
	struct Matrix3D sum;
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



struct myRender_vector_4 Matrix3D_muilti(struct myRender_vector_4 vector, struct Matrix3D mat)
{
	struct myRender_vector_4 sum;
	sum.color = vector.color;
	for (int j = 0; j <4 ; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			sum.position[j] += vector.position [k] * mat.matrix [k][j];
		}
	}
	return sum;
}



struct Matrix3D Matrix3D_add(struct Matrix3D mat1, struct Matrix3D mat2)
{
	struct Matrix3D sum;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			sum.matrix[i][j] = mat1.matrix[i][j] + mat2.matrix[i][j];
		}
	}
	return sum;
}