#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include "omp.h"

#define MAX_STEPS 100
double values_of_time[MAX_STEPS];

enum live {DEAD, LIVE};

void print(enum live** arr, int n, int m);
void init(enum live** arr, int n, int m);

void step(enum live** arr, int n, int m);
int stopGame(enum live** arr, int n, int m);
enum live changeSquare(enum live** arr, int n, int m, int x, int y);
enum live rulesOfLive(enum live** arr, int n, int m, int x, int y, int counter);
void lifeCircle(enum live** arr, int n, int m);
double timer(void(*f)( enum live** arr, int n, int m) , enum live** arr, int n, int m) ;


void print(enum live** arr, int n, int m)
{
	system("clear");
	int i,j;
	for(i = 0; i < n; ++i)
	{
		for(j = 0; j < m; ++j)
		{
			if(arr[i][j] == DEAD)
			{
				printf("\033[1;41m%c \033[0m", '*');
			}
			else if(arr[i][j] == LIVE)
			{
				printf("\033[1;42m%c \033[0m", ' ');
			}
		}
		printf("\n");
	}
}
void init(enum live** arr, int n, int m)
{
	int i,j;
	for(i = 0; i < n; ++i)
	{
		for(j = 0; j < m; ++j)
		{
			arr[i][j] = DEAD;
		}
	}
	int a = 1; int b = 1;
	printf("enter live fields while a!= -1 b!= -1");
	while(1)
	{
		scanf("%d%d", &a,&b);
		if(a < 0 || b < 0 || a>n-1 || b > m-1) break;
		arr[a][b] = LIVE;
	}
}
int stopGame(enum live** arr, int n, int m)
{
	int i, j;
	int flag = 0;
	for(i = 0; i < n; ++i)
	{
		for(j = 0; j < m; ++j)
		{
			if(arr[i][j] == LIVE) flag = 1;
//return 0;
		}
	}
	if(flag == 0) return 1;
	return 0;
//	return 1;
}
void step(enum live** arr, int n, int m)
{
	enum live newArr[n][m];
	int i,j;
	for(i = 0; i < n; ++i)
	{
		for(j = 0; j < m; ++j)
		{
			newArr[i][j] = changeSquare(arr, n, m, j, i);
		}
	}
	for( i = 0; i < n; ++i)
	{
		for( j = 0; j < m; ++j)
		{
			arr[i][j] = newArr[i][j];
		}
	}
}
enum live changeSquare(enum live** arr, int N, int M, int x, int y)
{
     int X[8] = {-1 , 0 , 1 , -1, 1, -1, 0, 1};
     int Y[8] = {-1, -1 ,-1,   0, 0, 1,  1, 1};
     int counter = 0;
     for(int k = 0; k < 8; k++)
     {
         if( (x + X[k] < M) && (x + X[k] > -1) &&(y + Y[k]< N) && (y + Y[k] > -1)) // map life
         {   
             if(arr[y + Y[k]][x + X[k]] == LIVE)
             {   
                 counter++;
             }
         }
         else if(x + X[k] == M && y + Y[k] != N && y + Y[k] != -1) //sphere life begin
         {   
             if( arr[y + Y[k]][0] == LIVE)
             {   
                 counter++;
             }
         }
         else if(x + X[k] == -1 && y + Y[k] != N && y + Y[k] != -1)
         {   
             if( arr[y + Y[k]][N-1] == LIVE)
             {   
                 counter++;
             }
         }
         else if(y + Y[k] == N && x + X[k] != M && x + X[k] != -1)
         {   
             if(arr[0][x+X[k]] == LIVE)
             {   
                 counter++;
             }
         }
         else if(y + Y[k] == -1 && x + X[k] != M && x + X[k] != -1)
         {
             if(arr[N-1][x+X[k]] == LIVE)
             {
                 counter++;
             }
         }
         else if(x + X[k] == -1 && y + Y[k] == -1)
         {
            if( arr[N-1][M-1] == LIVE)
             {
                 counter++;
             }
         }
         else if(x + X[k] == M && y + Y[k] == -1)
         {
             if(arr[N-1][0] == LIVE)
             {
                 counter++;
             }
         }
         else if(x + X[k] == M && y + Y[k] == M)
         {
             if(arr[0][0] == LIVE)
             {
                 counter++;
             }
         }
         else if( x + X[k] == -1 && y + Y[k] == M)
         {
             if(arr[0][M-1] == LIVE)
             {
                 counter++;
             }
         }  // sphere life end
     }
 //printf("x%d y%d counter%d\n",x , y, counter);
     return rulesOfLive(arr, N, M, x, y, counter);   
}
enum live rulesOfLive(enum live** arr, int n, int m, int x, int y, int counter)
 {
     if( arr[y][x] == LIVE && counter < 2 )
      {
                 return DEAD;
     }
     else if( arr[y][x] == LIVE && counter > 3)
     {
         return DEAD;
     }
     else if(arr[y][x] == DEAD && counter == 3)
     {
         return LIVE;
     }
 }
void lifeCircle(enum live** arr, int n, int m)
{
	int a;int i = 0;
	while(!stopGame(arr, n, m) && i < MAX_STEPS)
	{
//		print(arr, n, m);
//		printf("%lf\n",timer(step,arr, n, m));
		values_of_time[i++] = timer(step, arr, n, m);
//		scanf("%d", &a);
	}
}

double timer(void(*f)( enum live** arr, int n, int m) , enum live** arr, int n, int m)
{
     double t1 = omp_get_wtime();
     f(arr, n, m);
     double t2 = omp_get_wtime();
     return t2-t1;
}

int main(void)
{
	//init
	int n, m;
	printf("enter length and width\n");
	scanf("%d%d", &n, &m);
	enum live** arr = (enum live**)malloc(n * sizeof(enum live*));
	int i;
	for(i = 0; i < n; ++i)
	{
		arr[i] = (enum live*)malloc(m*sizeof(enum live));
	}
	init(arr, n, m);
//	float begin = clock();
	print(arr, n, m);
	printf(" all %lf\n", timer(lifeCircle,arr, n, m));
	for(i = 0; i < MAX_STEPS; ++i)
	{
		printf("num %d  %lf\n", i, values_of_time[i]); 
	}
//	print(arr, n, m);
//	float end = clock();
//	printf("%f\n", (end - begin)/CLOCKS_PER_SEC);
	//end of init
	//free
	for(i = 0; i < n; ++i)
	{
		free(arr[i]);
	}
	free(arr);
	// end of free
	return 0;
}
