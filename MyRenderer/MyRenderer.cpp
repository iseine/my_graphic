#include <iostream>
#include <cmath>
#include<array>
#include"MyMath.h"
#include"transform.h"
#define _USE_MATH_DEFINES

using namespace std;




int main()
{
	struct myRender_vector_4 cube[8];
	cube[0].position = { 0,0,0,1 }; cube[0].color = { 255,0,0 };
	cube[1].position = { 1,0,0,1 }; cube[1].color = { 0,255,0 };
	cube[2].position = { 1,0,1,1 }; cube[2].color = { 0,0,255 };
	cube[3].position = { 0,0,1,1 }; cube[3].color = { 255,255,0};
	cube[4].position = { 0,1,0,1 }; cube[4].color = { 0,255,255};
	cube[5].position = { 0,1,1,1 }; cube[5].color = { 255,0,255};
	cube[6].position = { 1,1,0,1 }; cube[6].color = { 0,0,0 };
	cube[7].position = { 1,1,1,1 }; cube[7].color = { 255,255,255};

	struct myRender_triangle cube_index[12];
	cube_index[0].index = { 0,1,2 }; cube_index[1].index = { 2,3,0 };//背面
	cube_index[2].index = { 0,4,5 }; cube_index[3].index = { 5,3,0 };//左侧
	cube_index[4].index = { 1,6,7 }; cube_index[5].index = { 7,2,1 };//右侧
	cube_index[6].index = { 5,7,2 }; cube_index[7].index = { 2,3,5 };//上面
	cube_index[8].index = { 4,6,1 }; cube_index[9].index = { 1,0,4 };//下面
	cube_index[10].index = { 4,6,7 }; cube_index[11].index = { 7,5,4 };//正面

	struct camera_set myCamera;
	myCamera.position = { 2,2,5,0 };
	myCamera.lookat = { 0.5, 0.5, 0.5, 0 };
	myCamera.up = { 0,1,0,0 };

	MVP_trans(myCamera, cube, sizeof(cube)/sizeof(cube[0]));


}