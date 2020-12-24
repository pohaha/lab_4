#pragma once
#include <cstring>
#include <iostream>



class Double_Vector
{

private:
	double* data = nullptr;
	std::size_t length = 0;

public:

//this is a method for adding elements to our vector
	void add(const double& n_data)
	{
		double* temp = new double[length + 1];

		std::memcpy(temp, data, sizeof(double) * length);
		delete[] data;

		data = temp;
		data[length++] = n_data;
	}


//this is a method for showing all contained elements
	void show(bool mode=true)
	{
		if(mode) std::cout << "array contents are: " << std::endl;
		for (unsigned int i = 0; i < length; i++)
		{
			if (mode) std::cout << "Element number " << i + 1 << " : ";
				std::cout << data[i] << '\t';
			if(mode)	std::cout<< std::endl;
		}

	}

//vector c-tors, d-tors
	//copy c-tor
	Double_Vector(const Double_Vector& target)
	{
		length = target.length;
		data = new double[length];
		std::memcpy(data, target.data, sizeof(double) * length);
	}
	//empty vector c-tor
	Double_Vector()
	{
	//this was used for tests
		//std::cout << "vector has been created - empty" << std::endl;
	}

	//c-tor with element initialization as a placeholder
	Double_Vector(std::size_t length, double placeholderElement): length(length)
	{
		data = new double[length];
		for (unsigned int i = 0; i < length; i++)
			data[i] = placeholderElement;
		//this was used for tests
			//std::cout << "vector has been created" << std::endl;
	}
		
	Double_Vector(double* n_Data, std::size_t n_Size):length(n_Size)
	{
		data = new double[length];
		std::memcpy(data, n_Data, length * sizeof(double));
	}
	//vector d-tor for cleaning the memory
	~Double_Vector()
	{
		delete[] data;
		//this was used for tests
			//std::cout << "vector has beed deleted" << std::endl;
	}


//tecnical vector operations
	// indexing operation
	double& operator[](unsigned int index)
	{
		if (index < length)
			return data[index];
		else
		{
			std::cout << "access violation - no such element yet" << std::endl;
			return data[0];
		}
	}
	const double& operator[](unsigned int index) const
	{
		if (index < length)
			return data[index];
		else
		{
			std::cout << "access violation - no such element yet" << std::endl;
			return data[0];
		}
	}

	void operator=(const Double_Vector& two)
	{
		length = two.length;
		double* n_data = new double[length];
		memcpy(n_data, two.data, sizeof(double) * length);
		data = n_data;
		
	}

	
//usefull vector operations

	//summ of 2 equally-lengthed vectors
	Double_Vector operator+ (const Double_Vector& two)
	{
		if (length != two.length) {
			std::cout << "Error! sizes are mismatched!" << std::endl;
			return *this;
		}
		Double_Vector* outputVector=new Double_Vector(length,0.f);
		for (unsigned int i = 0; i < length; i++)
		{
			outputVector->data[i] = outputVector->data[i] + data[i] + two[i];
		}
		return *outputVector;
	};

	//vector-number multiplication
	Double_Vector operator* (double number)
	{
		Double_Vector* outputVector=new Double_Vector(length, 0.f);
		for (unsigned int i = 0; i <length; i++) {
			(*outputVector)[i] = (*outputVector)[i] + data[i] * number;
		}
		return *outputVector;
	};

	unsigned int getSize()
	{
		return length;
	}
	
};



