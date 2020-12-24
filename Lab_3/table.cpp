#include "table.h"
#include <iostream>

//---check 
Table::Table(bool fresh)
{
	if (fresh)
	{
		std::cout << "input the size of the table: ";
		std::cin >> size;
		columns = new Double_Vector[2];
		columns[0] = Double_Vector(size, 0);
		columns[1] = Double_Vector(size, 0);
		std::cout << "Input the table elements:\ninput the values of x:\n";
		for (unsigned int i = 0; i < size; i++)
		{
			std::cin >> columns[0][i];
		}
		std::cout << "input the corresponding values of y:\n";
		for (unsigned int i = 0; i < size; i++)
		{
			std::cin >> columns[1][i];
		}

	}
}
void Table::show()
{
	std::cout << "X=[";
	for (unsigned int i = 0; i < size; i++)
	{
		std::cout << columns[0][i] << "\t";
	}
	std::cout << "]" << std::endl;
	std::cout << "Y=[";
	for (unsigned int i = 0; i < size; i++)
	{
		std::cout << columns[1][i] << "\t";
	}
	std::cout << "]" << std::endl;
}
void Table::sort()
{
	for(int j=size-1;j>0;j--)
		for(unsigned int i=0;i<j;i++)
		if (columns[0][i] > columns[0][i + 1])
		{
			double temp = columns[0][i];
			columns[0][i] = columns[0][i + 1];
			columns[0][i + 1] = temp;
			temp = columns[1][i];
			columns[1][i] = columns[1][i + 1];
			columns[1][i + 1] = temp;
		}
}
Table Table::operator=(const Table& other)
{
	size = other.size;
	columns = new Double_Vector[2];
	columns[0] = other.columns[0];
	columns[1] = other.columns[1];
	return *this;
}
void Table::add(const double& x_val, const double& y_val)
{
	size++;
	columns[0].add(x_val);
	columns[1].add(y_val);
}
Table Table::Linear(double* values, const unsigned int& count)
{
	Table output = *this;
	for (unsigned int i = 0; i < count; i++)
	{
		double y_val;
		unsigned int id_before = decide_id(id_modes::before, values[i]);
		unsigned int id_after = decide_id(id_modes::after, values[i]);
		y_val = columns[1][id_before] + (columns[1][id_after] - columns[1][id_before]) / (columns[0][id_after] - columns[0][id_before]) * (values[i] - columns[0][id_before]);
		output.add(values[i], y_val);
	}
	output.sort();
	return output;
}

Table Table::Canon(double* values, const unsigned int& count)
{
	Table output = *this;
	Double_Vector placeholder = Vandermond().solveFor(columns[1]);
	Matrix CanonPolynomKoefficients(&placeholder,1);
	Matrix NewElementGrades(BasicMatrixes::Zeros, size, 1);
	NewElementGrades[0][0] = 1;
	for (unsigned int j = 0; j < count;j++)
	{
		for (unsigned int i = 1; i < size; i++) NewElementGrades[i][0] = NewElementGrades[i - 1][0] * values[j];
		output.add(values[j], (CanonPolynomKoefficients * NewElementGrades).sum());
	}
	output.sort();
	return output;
}

Table Table::Lagrange(double* values, const unsigned int& count)	
{
	Table output = *this;
	Double_Vector LagrangeKoefficients;
	for (unsigned int i = 0; i < size; i++)
	{
		LagrangeKoefficients.add(1);
		for (unsigned int j = 0; j < size; j++)
			if (i != j) LagrangeKoefficients[i] *= columns[0][i] - columns[0][j];
		LagrangeKoefficients[i] = columns[1][i] / LagrangeKoefficients[i];
	}
	for (unsigned int element_id = 0; element_id < count; element_id++)
	{
		double n_element=0;
		for (unsigned int i = 0; i < size; i++)
		{
			double element_part=LagrangeKoefficients[i];
			for (unsigned int j = 0; j < size; j++)
				if (i != j) element_part *= values[element_id] - columns[0][j];
			n_element += element_part;
		}
		output.add(values[element_id], n_element);
	}
	output.sort();
	return output;
}
Table Table::Newton(double* values, const unsigned int& count)
{
	Table output = *this;
	double arg_step=0;
	bool first = true;
	for (unsigned int i = 0; i < size-1; i++)
	{
		if (first)
		{
			first = false;
			arg_step = columns[0][i + 1] - columns[0][i];
		}
		else
		{
			if (arg_step != columns[0][i + 1] - columns[0][i])
				std::cout << "unable to perform newton aproximation, uneven steps!\n";
			return output;
		}
		Double_Vector NewtonKoeficients;;
		for (unsigned int i = 0; i < size; i++)
		{
			double n_koeficient = Delta(&(columns[1]), i);
			for (unsigned int j = 1; j <= i; j++)
				n_koeficient /= j * arg_step;
			NewtonKoeficients.add(n_koeficient);
		}
		for (unsigned int element_id = 0; element_id < count; element_id++)
		{
			double n_y_value = Delta(&(columns[1]), 0);
			for (unsigned int i = 1; i < size; i++)
			{
				double tmp = 1;
				for (unsigned int j = 1; j <= i; j++)
					tmp = tmp * (values[element_id] - columns[0][j - 1]) / (j * arg_step);
				n_y_value += Delta(&(columns[1]), i) * tmp;
			}
			output.add(values[element_id], n_y_value);
		}
		output.sort();
		return output;
	}

}

double Table::Delta(Double_Vector* values, unsigned int lvl, unsigned int id, unsigned int step)
{
	if ((id == 0) & (lvl == 1)) return (*values)[1] - (*values)[0];
	if (id == 0) for (unsigned int i = 0; i <= lvl; i++) id += i;
	step++;
	if (lvl == 1) return (*values)[id]- (*values)[id - 1];
	else if (lvl == 0) return (*values)[0];
	else return Delta(values, lvl - 1, id - step, step) - Delta(values, lvl - 1, id - step - 1, step);
}

unsigned int Table::decide_id(id_modes mode, const double& value)
{
	if (mode)
	{
		for (unsigned int i = 0; i <size; i++)
			if (columns[0][i] > value)	return i;
	}
	else
	{
		for (unsigned int i = 0; i <size; i++)
			if (columns[0][i] > value)	return i-1;
	}
}

Matrix Table::Vandermond()
{
	Double_Vector* Polynoms = new Double_Vector[size];
	for (unsigned int i = 0; i < size; i++)
	{
		double element = 1;
		Polynoms[i].add(element);
		for (unsigned int j = 1; j < size; j++) Polynoms[i].add(element *= columns[0][i]);
	}
	return Matrix(Polynoms,size);
}
//---uncheck

Table::Table(const Table& other):size(other.size)
{
	columns=new Double_Vector[2];
	columns[0] = other.columns[0];
	columns[1] = other.columns[1];
}

Table::~Table()
{
	delete[] columns;
}


Table Table::aproximate(Aproximation_mode n_Mode)
{
	double* n_x_elements = nullptr;
	Table output;
	unsigned int count = 0;
	while (n_Mode == Aproximation_mode::MODE_UNSET)
	{
		unsigned int modeAsInt = 0;
		std::cout << "input the aproximation mode:" << std::endl;
		std::cout << "input 1 if you want linear aproximation" << std::endl;
		std::cout << "input 2 if you want canonical aproximation" << std::endl;
		std::cout << "input 3 if you want Lagrange aproximation" << std::endl;
		std::cout << "input 4 if you want Newton aproximation" << std::endl;
		std::cin >> modeAsInt;
		switch (modeAsInt)
		{
		case 0:
			n_Mode = Aproximation_mode::MODE_UNSET;
			break;
		case 1:
			n_Mode = Aproximation_mode::MODE_LINEAR;
			break;
		case 2:
			n_Mode = Aproximation_mode::MODE_CANON;
			break;
		case 3:
			n_Mode = Aproximation_mode::MODE_LAGRANGE;
			break;
		case 4:
			n_Mode = Aproximation_mode::MODE_NEWTON;
			break;
		default:
			std::cout << "something went wrong!" << std::endl;
			break;
		}
	}
	std::cout << "input the amount of elements you would like to add: ";
	std::cin >> count;
	n_x_elements = new double[count];
	std::cout << "input new x elements: \n";
	for (unsigned int i = 0; i < count; i++) std::cin >> n_x_elements[i];
	switch (n_Mode)
	{
	case MODE_LINEAR:
		output = Linear(n_x_elements, count);
		break;
	case MODE_CANON:
		output = Canon(n_x_elements, count);
		break;
	case MODE_LAGRANGE:
		output = Lagrange(n_x_elements, count);
		break;
	case MODE_NEWTON:
		output = Newton(n_x_elements, count);
		break;
	default:
		break;
	}
	return output;
}



