class Vector { //Create a Vector Class which perform vector addtion and multiplication
                           // In order to perform RK4
    public:
        Vector() = delete;                  // Do not allow vector without size
        Vector(const unsigned int pSize);   // Create zero vector of given size
        Vector(const Vector& pSrc);         // Copy another vector
        Vector(Vector&& pSrc);              // Move another vector

        double& operator[](unsigned int i); // Access entries in vector

        Vector& operator=(const Vector& pSrc); //Overloading Assignment operator
        Vector operator+(const Vector& pSrc); // Overloading addtion operator 
;
        Vector operator*(const double factor);// Overloading *
        Vector operator/(const double factor);// Overloading /


    private:
        double* values = nullptr;             // A pointer held privately
        unsigned int size = 0;
};

    