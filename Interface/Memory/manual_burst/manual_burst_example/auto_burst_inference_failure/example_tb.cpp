#include "example.h"
#include <iostream>
#define iter 10

int main()
{

	int size = 512;
	int in[size*iter];
	int a[size];

	int out[size*iter];

	for(int i=0;i<size;i++)
	{
		in[i]= i;
		a[i] =i;
	}

	krnl_transfer(in,out,size);

	int var ;
	for(int j=0; j<iter;j++)
	{
	for(int i=0;i<size;i++)
	{
		if(out[i] != a[i] )
        {
        std::cout <<"design fails" << " i " << i << " out "<< out[i] << " a " << a[i] << std::endl;
			return 1;
	
        }
    }
	}
	return 0;

}
