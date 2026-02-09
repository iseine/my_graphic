#include <iostream>
#include <cmath>
#define _USE_MATH_DEFINES

using namespace std;

struct Matrix2D {
	double matrix[3][3] = { 0 };
};



void makematrix(double mat1[9], struct Matrix2D *mat)
{
	int k = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat->matrix[i][j] = mat1[k];
			k++;
		}
	}
}

struct Matrix2D muilti(struct Matrix2D mat1, struct Matrix2D mat2)
{
	struct Matrix2D sum;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				sum.matrix[i][j] += mat1.matrix[i][k] * mat2.matrix[k][j];
			}
		}

	}
	return sum;
}

struct Matrix2D add(struct Matrix2D mat1, struct Matrix2D mat2)
{
	struct Matrix2D sum;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			sum.matrix[i][j] = mat1.matrix[i][j] + mat2.matrix[i][j];
		}
	}
	return sum;
}

void fina(struct Matrix2D mat, double point[3])
{
	double finally[3] = { 0,0,0 };
	std::cout << "(";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			finally[i] += point[j] * mat.matrix[i][j];
		}
	}
	std::cout << finally[0];
	std::cout << ",";
	std::cout << finally[1];
	std::cout << ")" << std::endl;

}
void test_matrix_multi() {
	double A_data[9] = { 1, 0, 0,
						0, 1, 0,
						0, 0, 1 }; // 单位矩阵
	double B_data[9] = { 2, 0, 0,
						0, 2, 0,
						0, 0, 1 }; // 缩放2倍

	Matrix2D A, B, C, D;
	makematrix(A_data, &A);
	makematrix(B_data, &B);

	C = muilti(A, B); // 单位矩阵乘任何矩阵应该等于原矩阵
	D = add(A, B);

	std::cout << "测试矩阵乘法 - 单位矩阵测试：" << std::endl;
	std::cout << "C[0][0]应该是2，实际是：" << C.matrix[0][0] << std::endl;
	std::cout << "C[1][1]应该是2，实际是：" << C.matrix[1][1] << std::endl;
	std::cout << "C[2][2]应该是1，实际是：" << C.matrix[2][2] << std::endl;
	std::cout << std::endl;
	std::cout << "测试矩阵加法 - 单位矩阵测试：" << std::endl;
	std::cout << "C[0][0]应该是3，实际是：" << D.matrix[0][0] << std::endl;
	std::cout << "C[1][0]应该是0，实际是：" << D.matrix[1][0] << std::endl;
	std::cout << "C[2][2]应该是2，实际是：" << D.matrix[2][2] << std::endl;
}
int main()
{
	test_matrix_multi();
	const double  PI = 3.14159265358979323846;
	double degrees = 30.0;
	double rad = degrees * PI / 180;

	std::cout << "点(2,0）绕原点旋转30度后缩放2倍再向右平移1是：" << endl;


	double point[3] = { 2,0,1 };

	double rotate[9] = { cos(rad),-sin(rad),0,
						 sin(rad),cos(rad),0,
						   0,0,0
	};
	double scale[9] = { 2,0,0,
						0,2,0,
						0,0,0 };
	double trans[9] = { 0,0,1,
						0,0,0,
						0,0,1 };

	Matrix2D rot, sca, tran;
	makematrix(rotate, &rot);
	makematrix(scale, &sca);
	makematrix(trans, &tran);
	std::cout << "经过旋转后的点：" << std::endl;
	fina(rot, point);
	std::cout << "经过缩放后的点：" << std::endl;
	fina(sca, point);
	std::cout << "经过旋转和缩放后的点：" << std::endl;
	fina(muilti(rot, sca), point);
	std::cout << "经过旋转缩放和平移后的点：" << std::endl;
	fina(add(muilti(rot, sca), tran), point);
	getchar();

}