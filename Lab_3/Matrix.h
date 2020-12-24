#pragma once
#include "Vector.h"
#include <iomanip>
enum class BasicMatrixes {
	Eye,
	Ones,
	Zeros
};
enum class LU_type {
	Lower,
	Upper
};
enum class Direction
{
	Line,
	Column
};
class Matrix
{
private :
	unsigned int rowNumber = 0;
	unsigned int columnNumber = 0;
	Double_Vector* Rows=nullptr;

public:

//matrix c-tors, d-tors
	//copy c-tor
	Matrix(const Matrix& target):
		rowNumber(target.rowNumber),columnNumber(target.columnNumber)
	{
		Rows = new Double_Vector[rowNumber];
		for (unsigned int i = 0; i < rowNumber; i++)
			Rows[i] = target.Rows[i];
	}
	Matrix(bool Construct=false)
	{
		if (Construct)
		{
			std::cout << "Input the height of a matrix: ";
			std::cin >> rowNumber;
			std::cout << "Input the width of a matrix: ";
			std::cin >> columnNumber;
			Rows = new Double_Vector[rowNumber];
			for (unsigned int i = 0; i < rowNumber; i++)
			{
				Rows[i] = Double_Vector(columnNumber, 0);
				std::cout << "Input row number " << i + 1 << std::endl;
				for (unsigned int j = 0; j < columnNumber; j++)
				{
					std::cin >> Rows[i][j];
				}
			}
		}
	}
	
	//matrix pre-defined
	Matrix(Double_Vector* n_data, unsigned int n_size)
	{
		rowNumber = n_size;
		columnNumber = n_data[0].getSize();
		Rows = new Double_Vector[rowNumber];
		for (unsigned int i = 0; i < rowNumber; i++)
			Rows[i] = n_data[i];
	}
	//basic matrixes constructor
	Matrix(BasicMatrixes type,unsigned int n_Rows, unsigned int n_Columns):rowNumber(n_Rows),columnNumber(n_Columns)
	{
		switch (type)
		{
		case BasicMatrixes::Eye:
			Rows = new Double_Vector[rowNumber];
			for (unsigned int i = 0; i < rowNumber; i++)
			{
				Rows[i] = Double_Vector(columnNumber, 0);
				Rows[i][i] = 1;
			}
			break;
		case BasicMatrixes::Ones:
			Rows = new Double_Vector[rowNumber];
			for (unsigned int i = 0; i < rowNumber; i++) Rows[i] = Double_Vector(columnNumber, 1);
			break;
		case BasicMatrixes::Zeros:
			Rows = new Double_Vector[rowNumber];
			for (unsigned int i = 0; i < rowNumber; i++) Rows[i] = Double_Vector(columnNumber, 0);
			break;
		default:
			break;
		}
	}

	//matrix d-tor
	~Matrix()
	{
		delete[] Rows;
	}

//matrix usefull operations
	//Vector from one col/line matrix
	Double_Vector VectorForm(Direction type, unsigned int number)
	{
		std::size_t size = (type == Direction::Column) ? (rowNumber) : (columnNumber);
		Double_Vector outputVector;
		for (unsigned int i = 0; i < size; i++)
			outputVector.add((type == Direction::Column) ? (Rows[i][number]) : (Rows[number][i]));
		return outputVector;
	}
	//LU eqating
	Matrix LU_form(LU_type type) const
	{
		if (rowNumber != columnNumber)
		{
			std::cout << "unable to count LU form, this is not a square matrix!";
			return Matrix(BasicMatrixes::Zeros,1,1);
		}
		Matrix LU_Form;
		LU_Form= *this;
		double m;
		for (unsigned int Counter = 0; Counter < columnNumber - 1; Counter++)
		{
			for (unsigned int rowCounter = Counter+1; rowCounter < rowNumber; rowCounter++)
			{
				m = LU_Form[rowCounter][Counter] / LU_Form[Counter][Counter];
				for (unsigned int columnCounter = Counter; columnCounter < columnNumber; columnCounter++)
				{
				if (columnCounter == Counter) 
					LU_Form[rowCounter][columnCounter] = ((type==LU_type::Lower)?(m):(0));
				else 
					LU_Form[rowCounter][columnCounter] =  LU_Form[rowCounter][columnCounter] - m * (LU_Form[Counter][columnCounter]);
				}
			}
		}
		if (type == LU_type::Lower)
		{
			for (unsigned int i = 0; i < rowNumber; i++)
			{
				for (unsigned int j = i; j < columnNumber; j++)
					LU_Form[i][j] = ((i == j) ? (1.0) : (0.0));
			}
		}
		return LU_Form;
	}
	//Solve for defined free members
	Double_Vector solveFor( const Double_Vector& free_Member) const
	{
		Double_Vector Answers(rowNumber, 0.f);
		Matrix LowerTriangle = LU_form(LU_type::Lower);
		Matrix UpperTriangle = LU_form(LU_type::Upper);
		for (unsigned int i = 0; i < rowNumber; i++)
		{
			
			Answers[i] = free_Member[i];
			for (unsigned int j = 0; j < i; j++) Answers[i] -= LowerTriangle[i][j] * Answers[j];
		}
		for (int i = rowNumber-1; i >= 0; i--)
		{
			for (int j = rowNumber - 1; j > i; j--) Answers[i] -= UpperTriangle[i][j] * Answers[j];
			Answers[i] = Answers[i] / UpperTriangle[i][i];
		}
		return Answers;

	}

	//matrix indexing operation
	Double_Vector& operator[](unsigned int index)
	{
		if (index < rowNumber)
			return Rows[index];
		else
		{
			std::cout << "access violation - no such element yet" << std::endl;
			return Rows[0];
		}
	}
	const Double_Vector& operator[](unsigned int index) const
	{
		if (index < rowNumber)
			return Rows[index];
		else
		{
			std::cout << "access violation - no such element yet" << std::endl;
			return Rows[0];
		}
	}

	//data information
	void show()
	{
		std::cout << "Matrix contents are:" << std::endl;
		std::cout << std::setprecision(10);
		for (unsigned int i = 0; i < rowNumber; i++)
		{
			Rows[i].show(false);
			std::cout << std::endl;
		}
	}

	//matrix*matrix multiplication
	Matrix operator*(const Matrix& two)
	{
		if (columnNumber == two.rowNumber)
		{
			Matrix outputMatrix(BasicMatrixes::Zeros, rowNumber, two.columnNumber);
			for (unsigned int rowCounter = 0; rowCounter < rowNumber; rowCounter++)
			{
				for (unsigned int columnCounter = 0; columnCounter < two.columnNumber; columnCounter++)
				{
					for (unsigned int compositionCounter = 0; compositionCounter < columnNumber; compositionCounter++)
						outputMatrix[rowCounter][columnCounter] += Rows[rowCounter][compositionCounter] * two.Rows[compositionCounter][columnCounter];
				}
			}
			return outputMatrix;
		}
		else
		{
			std::cout << "Unable to multiply, row/col sizes mismatch!" << std::endl;
			return *this;
		}
	}

	// matrix*scalar multiplication
	Matrix operator* (const double& scalar)
	{
		Matrix Result = (*this);
		for (unsigned int i = 0; i < rowNumber; i++)
			for (unsigned int j = 0; j < columnNumber; j++)
				Result[i][j] *= scalar;
		return Result;
	}

	//matrix+matrix
	Matrix operator+(const Matrix& other)
	{
		Matrix result = (*this);
		if ((rowNumber == other.rowNumber) && (columnNumber == other.columnNumber))
			for (unsigned int i = 0; i < rowNumber; i++)
				result[i] = result[i] + other[i];
		return result;
	}

	//transpose matrix
	Matrix transpose()
	{
		Matrix outputMatrix= Matrix(BasicMatrixes::Zeros,columnNumber,rowNumber);
		for (unsigned int rowCounter = 0; rowCounter < outputMatrix.rowNumber; rowCounter++)
		{
			for (unsigned int columnCounter = 0; columnCounter < outputMatrix.columnNumber; columnCounter++)
			{
				outputMatrix[rowCounter][columnCounter] = Rows[columnCounter][rowCounter];
			}
		}
		return outputMatrix;
	}

	//sum of elements
	double sum()
	{
		double summ = 0;
		for (unsigned int i = 0; i < rowNumber; i++)
			for (unsigned int j = 0; j < columnNumber; j++)
				summ += Rows[i][j];
		return summ;
	}
//nesessary operations
	//equals operator
	Matrix operator=(const Matrix& two)
	{
		rowNumber = two.rowNumber;
		columnNumber = two.columnNumber;
		Double_Vector* n_data = new Double_Vector[rowNumber];
		memcpy(n_data, two.Rows, sizeof(Double_Vector) * rowNumber);
		Rows = n_data;
		for (unsigned int i = 0; i < rowNumber;i++)
		{
			n_data[i] = two.Rows[i];
		}
		return *this;
	}
	
	//setters|getters
	int getSize()
	{
		return rowNumber;
	}
};
