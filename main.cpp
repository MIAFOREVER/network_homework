#include<iostream>
#include<vector>
using namespace std;
int main()
{
	int q[4][8];
	int q_num[4];
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			q[i][j] = 0;
		}
		q_num[i] = 0;
	}
	int min = 0;
	int car = 48;
	while(1)
	{
		if(min % 15 == 0 && car != 0)
		{
			q[0][q_num[0]] = 0;
			q_num[0]++;
			car --;
		}
		for(int i = 1; i < 4; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				if(q[i][j] > 0)
					q[i][j] ++;
				std::cout<<q[i][j]<<" ";
			}
			std::cout<<std::endl;
		}
		for(int i = 0; i < 5; i++)
		{
			if(q[3][i] == 91)
			{	
				q[3][i] = 0;
				q_num[3] --;
			}
		}
		for(int i = 0; i < 8; i++)
		{
			if(q[2][i] >= 121)
			{
				//q[2][i] = 0;
				//q_num[2] --;
				for(int j = 0; j < 5; j++)
				{
					if(q[3][j] == 0)
					{
						q[3][j] = 1;
						q[2][i] = 0;
						q_num[2] --;
						q_num[3] ++;
						break;
					}
				}
			}
		}
		//q[1][1] =2;
		for(int i = 0; i < 3; i++)
		{
			if(q[1][i] >= 61)
			{
				for(int j = 0; j < 8; j++)
				{
					if(q[2][j] == 0)
					{
						q[2][j] = 1;
						q[1][i] = 0;
						q_num[1]--;
						q_num[2]++;
						break;
					}
				}
			}
		}
		for(int i = 0; i < 3; i++)
		{
			if(q_num[1] < 3 && q_num[0] > 0)
			{
				if(q[1][i] == 0)
				{
					q[1][i] = 1;
					q_num[0] --;
					q_num[1] ++;
				}
			}
		}
		std::cout<<q_num[0]<<" "<<q_num[1]<<" "<<q_num[2]<<" "<<q_num[3]<<endl;
		if(q_num[0] == 0 && q_num[1] == 0 && q_num[2] == 0 && q_num[3] == 0 && car == 0)
			break;
		min ++;	
	}
	std::cout<<min<<endl;
	return 0;
}
