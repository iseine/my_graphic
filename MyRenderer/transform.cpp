#include <iostream>
#include <cmath>
#include<array>
#include"MyMath.h"
#include"transform.h"
#define _USE_MATH_DEFINES

struct Matrix3D MVP_trans(struct camera_set camera, struct myRender_vector_4 *vector,int size)
{
	struct Matrix3D Unit;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				Unit.matrix [i][j] = 1;
			}
		}
	}

	struct Matrix3D M_trans;
	for (int i = 0; i < 4; i++)
	{
		Unit.matrix[i][3] = -camera.position[i];
	}
	

	


}