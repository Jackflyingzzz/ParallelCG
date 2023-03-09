#include <cmath>
#include "stage.h"
#include <iostream>
using namespace std;

stage ::  stage(const double hinput, const double vinput, const double minput, const double mrocket,  ///The constructor for inputing the parameters and data belongs to this stage.
                  const double Area, const double mfrate, const double rho, const double ga, const double dragco,
                 const double uexit, const double dtstep) : h(hinput), v(vinput), m(minput), mr(mrocket), A(Area), mfd(mfrate), ue(uexit),
                                                                                                      p(rho), g(ga), Cd(dragco), dt(dtstep){
                     
                 }

Vector stage:: Getstate(double time){ // The Getstate member function will perform RK4 at a specific time input. And return the
                                                                            // state of the rocket [h, v, m] using a Vector class.
      
      Vector k0(3); //Initialise k0, which is the solution at each time, wiht k0 = [h, v, m], and return k0 at the end.
      k0[0] = h;
      k0[1] = v;
      k0[2] = m;
      
      Vector k1(3); // Initialise k1, k2, k3, k4
      Vector k2(3);
      Vector k3(3);
      Vector k4(3);
      Vector kmedium(3);// kmedium is used to perform (k0 + 0.5 k1)

      
      /////////////////////////RK4 Implementation////////////////////////////////////////////////////////////////////////////////////////////////////////
      double steps = time/dt; //The number of steps we need to compute depends on "time" input of the member function, 
                                                      //"time"/dt = number of step
      for (int i =0; i<steps; ++i){ 
          
          k1[0] = dt*k0[1];
          k1[1] =  dt*(- g- 0.5*p*k0[1]*abs(k0[1])*Cd*A/k0[2] + (k0[1]/abs(k0[1]))*mfd*ue/k0[2]);
          k1[2] = dt* (-mfd);
          
          kmedium  = k0 + k1*0.5;
          
          k2[0] = dt*kmedium[1];
          k2[1] =  dt*(- g- 0.5*p*kmedium[1]*abs(kmedium[1])*Cd*A/kmedium[2] + (kmedium[1]/abs(kmedium[1]))*mfd*ue/kmedium[2]);
          k2[2] = dt* (-mfd);
          
          kmedium  = k0 + k2*0.5;
          
          k3[0] = dt*kmedium[1];
          k3[1] =  dt*(- g- 0.5*p*kmedium[1]*abs(kmedium[1])*Cd*A/kmedium[2] + (kmedium[1]/abs(kmedium[1]))*mfd*ue/kmedium[2]);
          k3[2] = dt* (-mfd);
          
          kmedium = k0 + k3;
          
          k4[0] = dt*kmedium[1];
          k4[1] =  dt*(- g- 0.5*p*kmedium[1]*abs(kmedium[1])*Cd*A/kmedium[2] + (kmedium[1]/abs(kmedium[1]))*mfd*ue/kmedium[2]);
          k4[2] = dt* (-mfd);
        
          k0 = k0 + k1/6 + (k2+k3)/3 + k4/6;
          
      }
      
      //std:: cout << "h = "<< k0[0] <<", v ="<< k0[1]<< ", m =" << k0[2]<< std::endl;
      
      return k0;
}



































