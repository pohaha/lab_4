#include "table.h"
#include "value.h"
#include <cstdlib>

int main()
{
	std::cout << std::setprecision(7);
	Table testTable(true);
	system("PAUSE");
	system("CLS");
	std::cout << "linear:" << std::endl;
	testTable.aproximate(Aproximation_mode::MODE_LINEAR).show();
	system("PAUSE");
	system("CLS");
	std::cout << "canon:" << std::endl;
	testTable.aproximate(Aproximation_mode::MODE_CANON).show();
	system("PAUSE");
	system("CLS");
	std::cout << "lagrange:" << std::endl;
	testTable.aproximate(Aproximation_mode::MODE_LAGRANGE).show();
	system("PAUSE");
	system("CLS");
	std::cout << "newton:" << std::endl;
	testTable.aproximate(Aproximation_mode::MODE_NEWTON).show();
	
}