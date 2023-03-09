#include <Vector.h>

class stage{ //Create a class called stage, which contain the enviromental and rocket parameters 
                        // of that stage.    
    
public:
    
    stage() = default; //Class constructor
    stage(const double hinput, const double vinput, const double minput, const double mrocket, 
                  const double Area, const double mfrate, const double rho, const double ga, const double dragco,
                 const double uexit, const double dtstep); //The constructor for inputing the parameters and data belongs to this stage.


        
    Vector Getstate(double time); // The member function of "stage" class, it retrieve the RK results at a specific "time" variable 
                                                                //  which return a Vector class type.
    
    
    

private:

// Private data of the class, which contain the enviromental and rocket parameters.
double h; 
double v;
double m;
double mr;
double A;
double mfd;
double ue;
double dt;
double p;
double g;
double Cd;
    
    


};









