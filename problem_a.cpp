#include<iostream>
#include<cmath>
#include<fstream>

#define TIME_SLICE 0.001
#define G 9.8
using namespace std;
void calculate(double& x, double& y, double& v_x, double& v_y, double& a, double& w)
{
	a += TIME_SLICE * w;
	double x_temp, y_temp;
	double v_y_temp;
	x_temp = v_x * TIME_SLICE;
	v_y_temp = v_y - G * TIME_SLICE;
	y_temp = (v_y - 0.5 * G * TIME_SLICE) * TIME_SLICE;
	v_y = v_y_temp;
	x += x_temp;
	y += y_temp;
}
int main()
{
	fstream fout("info.txt");
	//init position
	double x = 0, y = 2;
	double a1, a2, a3;
	double v, v_x, v_y;
	double w;
	a1 = 33;
	a2 = 4;
	a3 = a1 + a2;
	v = 31.5;
	w = 0;
	v_x = v * cos(a1 / 57.3);
	v_y = v * sin(a1 / 57.3);
	while(1)
	{
		calculate(x, y, v_x, v_y, a3, w);
		fout<<x<<" "<<y<<endl;
		if((y - 0.6 * sin(a3)) < 0.01)
			break;
	}
	cout<<x<<endl;
	return 0;
}	
