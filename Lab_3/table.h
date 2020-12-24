#pragma once
#include "value.h"
#include "Matrix.h"
#include "Vector.h"
enum Aproximation_mode
{
	MODE_UNSET,
	MODE_LINEAR,
	MODE_CANON,
	MODE_LAGRANGE,
	MODE_NEWTON
};
enum id_modes
{
	before,
	after
};
class Table
{
private:
	Double_Vector* columns = nullptr;
	unsigned int size = 0;
	Aproximation_mode Default_Mode = Aproximation_mode::MODE_UNSET;
public:
//Table c-tors, d-tors
	//default c-tor + user input
	Table(bool fresh = false);
	//copy c-tor
	Table(const Table& other);
	//Table d-tor
	~Table();
//nesessities
	Table operator=(const Table& other);
//task stuff
	//main aproximation function
	Table aproximate(Aproximation_mode n_Mode=Aproximation_mode::MODE_UNSET);
	//adding an element to a table
	void add(const double& x_val, const double& y_val);
	//show as an input to the scilablab
	void show();
	//sort the table
	void sort();


	Matrix Vandermond();
private:
	Table Linear(double* values, const unsigned int& count);
	Table Canon(double* values, const unsigned int& count);
	Table Lagrange(double* values, const unsigned int& count);
	Table Newton(double* values, const unsigned int& count);
	double Delta(Double_Vector* values, unsigned int lvl, unsigned int id = 0, unsigned int step = 0);
	unsigned int decide_id(id_modes mode, const double& value);
};

