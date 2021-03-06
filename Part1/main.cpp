#include <iostream>
#include "tsp.h"
using namespace std;

int main()
{
	
	srand(time(NULL)); // random numbers
	
	int input[210]={0,1,7,9,7,3,3,0,9,1,6,7,9,1,4,9,4,6,8,1,0,7,2,4,0,5,2,7,8,6,1,9,2,0,6,4,4,0,7,0,7,6,9,1,1,8,1,4,3,4,8,9,4,8,5,3,6,0,6,3,7,4,9,5,8,4,1,4,3,2,8,4,3,3,0,7,6,6,6,5,6,7,9,7,1,4,5,9,7,5,0,8,9,4,1,2,3,4,6,8,8,4,8,8,0,0,6,9,5,3,0,6,3,4,3,6,4,5,5,0,2,0,2,6,1,9,0,2,4,0,7,4,0,4,6,3,3,9,3,8,1,6,4,9,0,6,4,3,1,0,4,2,9,7,4,0,3,6,0,7,2,2,6,3,4,0,0,4,9,7,8,5,3,1,0,0,9,4,9,4,2,9,0,4,7,0,3,0,1,0,5,4,3,6,1,0,2,9,3,7,0,6,0,0,0,2,1,0,7,0};
	

	Graph * graph4 = new Graph(20, 0);
	
	int cnt=0;
	for(int i=0;i<20;i++){
		for(int j=i;j<20;j++){
			if(input[cnt]){
			graph4->addEdge(i,j ,input[cnt]);
			graph4->addEdge(j,i,input[cnt]);
	}
	cnt++;
	}
	}
	
	Genetic genetic(graph4, 10, 15000, 5, true);

	genetic.run(); 
	
	
	return 0;
}
