#include "ScopedPointer.h"


int main()
{
	ScopedPointer<int> myUniquePtr1(new int(5));
	std::cout << "*myUniquePtr1 = " << *myUniquePtr1 << "\n";

	auto myUniquePtr2 = std::move(myUniquePtr1);
	std::cout << "*myUniquePtr2 = " << *myUniquePtr2 << "\n";

	ScopedPointer<int> myUniquePtr3;
	myUniquePtr3 = std::move(myUniquePtr2);
	std::cout << "*myUniquePtr3 = " << *myUniquePtr3 << "\n";
}