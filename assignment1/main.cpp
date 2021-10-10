#include <iostream>
#include "LinkedList.h"

using namespace std;

int main()
{
	LinkedList<int> LL;
	for(int i=1; i<11; i++)
		LL.AddAtHead(i);
	cout<<"AddAtHead : "<<endl;
	LL.Print(); // (10,9,8,7,6,5,4,3,2,1)
	
	cout<<"\nGet : "<<endl<<LL.Get(2)<<endl; // 8
	
	cout<<"\nAddAtIndex : "<<endl;
	LL.AddAtIndex(2,8);
	LL.Print(); // (10,9,8,8,7,6,5,4,3,2,1)
	
	cout<<"\nDeleteAtIndex : "<<endl;
	LL.DeleteAtIndex(3);
	LL.Print(); // (10,9,8,7,6,5,4,3,2,1)

	cout<<"\nDeleteValue : "<<endl;
	LL.DeleteValue(9);
	LL.Print(); // (10,8,7,6,5,4,3,2,1)
	
	cout<<"\nMoveToHead : "<<endl;
	LL.MoveToHead(2);
	LL.Print(); // (2,10,8,7,6,5,4,3,1)
	
	cout<<"\nRotate : "<<endl;
	LL.Rotate(2);
	LL.Print(); // (3,1,2,10,8,7,6,5,4)
	
	cout<<"\nAddAtHead : "<<endl;
	LL.AddAtHead(4);
	LL.Print(); // (4,3,1,2,10,8,7,6,5,4)
	
	cout<<"\nReduce : "<<endl;
	LL.Reduce();
	LL.Print(); // (4,3,1,2,10,8,7,6,5)
	
	cout<<"\nAddAtIndex : "<<endl;
	LL.AddAtIndex(7,10); 
	LL.Print(); // (4,3,1,2,10,8,7,10,6,5)

	cout<<"\nK_Reverse : "<<endl;
	LL.K_Reverse(3);
	LL.Print(); // (1,3,4,8,10,2,6,10,7,5)
	cout<<LL.Size()<< endl; // 10

	cout<<"\nEvenOddSeparateSort : "<<endl;
	LL.EvenOddSeparateSort();	
	LL.Print(); // (2,4,6,8,10,10,7,5,3,1)
	cout<<LL.Size()<< endl; // 10

	cout<<"\nCleanUp : "<<endl;
	LL.CleanUp(); 
	cout<<LL.Size()<<endl; // 0
}