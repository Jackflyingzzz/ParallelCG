#include <cmath>
#include <cstring>
#include <stdexcept>
using namespace std;

#include "Vector.h"


Vector::Vector(const unsigned int pSize) {  //Creates a vector of given size
    if (pSize == 0) throw std::logic_error("Vector size must be > 0."); // Throw error if the input size = 0

    size = pSize;
    values = new double[pSize];
    memset(values, 0, pSize*sizeof(double));
}


Vector::Vector(const Vector& rhs) {  // Copies an existing vector. 
    size = rhs.size;
    values = new double[rhs.size];
    memcpy(values, rhs.values, size*sizeof(double));
}


Vector::Vector(Vector&& rhs) { //Moves a existing vector.
    size = rhs.size;
    values = rhs.values;
    rhs.values = nullptr;
    rhs.size = 0;
}

double& Vector::operator[](unsigned int i) { //Access entries in the vector.
    if (i >= size) throw std::logic_error("Index out of range.");

    return values[i];
}


Vector& Vector::operator=(const Vector& rhs) {// Overloading assignment operation.
   
    memcpy(values, rhs.values, size*sizeof(double)); // Copy the values to private pointer.
    
    return *this;
}

Vector Vector::operator*(const double constant) {//Overloading multiplication operation, 
                                                                                             //there is no vector dot product in RK4 hence multiply by a constant 
    Vector result(size);
    for (int i = 0; i < size; ++i) {
        result.values[i] = values[i] * constant;
    }
    return std::move(result);
}

Vector Vector::operator/(const double constant) { //Overloading division operation.
    Vector result(size);
    for (int i = 0; i < size; ++i) {
        result.values[i] = values[i] / constant;
    }
    return std::move(result);
}



Vector Vector::operator+(const Vector& rhs) { // Overloading addition operation.
    if (rhs.size != size) throw std::logic_error("Vectors are different size.");// To ensure the size of two vector are the same.
    
    Vector result(size);
    for (int i = 0; i < size; ++i) {
        result.values[i] = values[i] + rhs.values[i];
    }
    return std::move(result);
}




