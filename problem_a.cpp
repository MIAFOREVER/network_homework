#include<iostream>
#include<cmath>
#include<fstream>
#define TIME_SLICE 0.01
#define PI 3.1415926
#define G 9.8
#define n 0.0000184
#define q 0.001184
#define S 20980
using namespace std;
double sign(double i)
{
	if(i > 0)
		return 1.0;
	else
		return -1.0;
}
double _abs(double i)
{
	if(i < 0)
		return -i;
	else
		return i;
}
void calculate(double& x, double& y, double& v_x, double& v_y, double& alpha, double& a, double& w, double& k)
{
	a += TIME_SLICE * w;
	double x_temp, y_temp;
	double v, v_x_temp, v_y_temp;
	double a_sum, a_x_temp, a_y_temp;
	v = sqrt(v_x * v_x + v_y * v_y);
	a_sum = 0; 
	a_x_temp = 0 - a_sum * cos(alpha);
	a_y_temp = -G - sign(v_y) * a_sum * sin(alpha);
	x_temp = v_x * TIME_SLICE + 0.5 * a_x_temp * TIME_SLICE * TIME_SLICE;
	v_x_temp = v_x + a_x_temp * TIME_SLICE;
	v_y_temp = v_y + a_y_temp * TIME_SLICE;
	y_temp = v_y * TIME_SLICE + 0.5 * a_y_temp * TIME_SLICE * TIME_SLICE;
	v_x = v_x_temp;
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
	double d;
	a1 = 34.0;
	a2 = 1.0;
	v = 30.1;
	int num;
	double min = 0, answer, a_temp;
	cin >> num;
	//cin >> a1 >> a2 >> v;
	while(num)
	{
	cin >> a1 >> a2 >> v >> answer;
	min = 0; 
	a3 = a1 + a2;
	w = 0;
	v_x = v * cos(a1 / 57.29578);
	v_y = v * sin(a1 / 57.29578);
	d = 1.22;
	double k = 0;
	for(double a = 0; a < 20; a += 0.1)
	{
		v_x = v * cos(a1 / 57.29578);
		v_y = v * sin(a1 / 57.29578);
		x = 0;
		y = 2;
		while(1)
		{
			calculate(x, y, v_x, v_y, a1, a3, w, a);
			fout<<x<<" "<<y<<endl;
			if((y - d * sin(a3 / 57.29578)) < 0.01)
			{
				double t1 = x - answer;
				double t2 = min - answer;
				if(_abs(t1) < _abs(t2))
				{
					a_temp = a;
					min = x;
				}
				//cout<<"x:\t"<<x<<"\ta:\t"<<a<<endl;
				break;
			}
		}
	}
	end:
	cout<<"num:\t"<<24-num<<"\tk:\t"<<a_temp<<"\tdis:\t"<<min<<endl;
	num--;
	}
	return 0;
}	
