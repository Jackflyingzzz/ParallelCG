#include "stage.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


int main()
{
 
    ifstream vMyFile("parameters.txt");  // Input Stream.
    double rho, g, Cd, v0, h0; // Initialising enviromental parameters.
    vector <double> m0, mr, A, mfd, ue, dt; // Create STL vectors for each stage-parameters.
    double temp1,temp2,temp3,temp4,temp5,temp6; 
    
//////////////////////////////////////////Read Parameters/////////////////////////////////////////////////////////////////////////////

    if (vMyFile.good()){ 
        
        vMyFile >> rho >> g >> Cd>> v0 >> h0; // Read the first line of the file which is enviromental parameters
                                                                                    // stored as double.
        
        while (true) {

            vMyFile >> temp1  >> temp2 >> temp3 >> temp4 >> temp5 >> temp6;
            m0.push_back(temp1);   // Store the variable read from the file to the vector of the stage-parameters.
            mr.push_back(temp2);
            A.push_back(temp3);
            mfd.push_back(temp4);
            ue.push_back(temp5);
            dt.push_back(temp6);
            
            if (vMyFile.eof()) { //Break when reahcing the end of the file.
                break;
            }
        }
        vMyFile.close(); // Close the file.

    }
    
    else {
        cout <<"Failed to open"<<endl;
    }
////////////////////////////////////// Initialise Output Stream//////////////////////////////////////////////////////////////////////

    ofstream vOut("output.txt", ios::out | ios::trunc);
    vOut << setw(30) << "h" <<setw(30)<<"v"<<setw(30)<< "m" << setw(30) << "t"<<endl;

////////////////////////////////////////////////STAGE 1/////////////////////////////////////////////////////
    
    stage s1(h0,v0, m0[0]+m0[1], mr[0]+mr[1],A[0], mfd[0], rho,g,Cd,ue[0],dt[0]); // Create stage 1 named "s1"
    Vector state1(3); // v = [ h v m ], Using the Vector class to receive the results from s1.Getstate member function. 
    state1 = s1.Getstate(0);  // Initialise Vector state1
    double time =0, totaltime=0;         // Initialise "time" in this stage, and the "totaltime" throughout all stages.
    double count  = 1;
    while (state1[2] >= (m0[1]+mr[0])){    // Implement the loop until the fuel in first stage burned out, the remaining mass will be total mass of the second stage 
                                                                     // and the rocket mass of the first stage.
        if ((state1[2] - (m0[1]+mr[0])) <300&& count == 1)  {
            count += 1;
            dt[0] = 0.1;
            
        }

        time += dt[0];                        //  We want to retrieve the state1 data(h,v, m) at a specific time, the class member function "stage.Getstate()" will return the state1 by inputing the time.
        totaltime += dt[0];             //  So we increment the time at each loop, and retrive the data from the member function.
        state1 = s1.Getstate(time);  // Retrive the data from the member function. 
        
        vOut.precision(5);
        vOut << setw(30) << state1[0]<< setw(30)<< state1[1]<< setw(30)<< state1[2] << setw(30) << totaltime<<endl;// Output the state1 data at this particular stage.

    }
    
    ///////////////////////////////////////////// STAGE 2 ////////////////////////////////////////////////////
    
    stage s2(state1[0],state1[1], m0[1], mr[1],A[1], mfd[1], rho,g,Cd,ue[1],dt[1]); // Create stage 2 named "s2", and repeat the logic and format in stage1
    Vector state2(3);
    state2 = s2.Getstate(0);
    time =0;
    count =1;
    while (state2[2] >= mr[1]){    // Keep solving the ode until the second stage fuel mas burned out. 
        
        if ((state2[2] - mr[1]) <300&& count == 1)  {
            count += 1;
            dt[1] = 0.2;
            
        }
        totaltime += dt[1];
        time += dt[1];
        state2 = s2.Getstate(time);
        
        vOut.precision(5);
        vOut << setw(30) << state2[0]<< setw(30)<< state2[1]<< setw(30)<< state2[2] << setw(30) << totaltime<<endl;

    }
    
    /////////////////////////////////////////STAGE 3 FREEFALL/////////////////////////////////////////
    
    stage s3(state2[0],state2[1], m0[2], mr[2],A[2], mfd[2], rho,g,Cd,ue[2],dt[2]); // Create stage 3 which is freefall, with no mass flow rate and exit velocity. 
    Vector state3(3);                                                                                                                 // the parameters of stage 3 is in the parameters file.
    state3 = s3.Getstate(0);
     time =0;
    double tempv, temph, vterminal, maxheight; // Initialise  varibale for terminal velocity and maximum height
    
    while (state3[0] >= 0){ // Keep solving the ODE until h = 0
        
        time += dt[2];
        totaltime += dt[2];
        temph =state3[0];// Record the temporary h and v to compare with the next h and v to find terminal velocity and max height.
        tempv = state3[1]; // Record the temporary h and v to compare with the next h and v to find terminal velocity and max height.

        state3 = s3.Getstate(time);
        
        if (std::abs(state3[1] - tempv) <= 0.1){ // While the velocity is not changing, the velocity is terminal velocity.
            
               vterminal = tempv;
        }
        
        if ((state3[0]- temph) >= 0 ){ //Keep recording the height until the rocket start falling.
                
                maxheight = state3[0];
            
        }
        
        vOut.precision(5);
        vOut << setw(30) << state3[0]<< setw(30)<< state3[1]<< setw(30)<< state3[2] << setw(30) << totaltime<<endl;
        
    
    }
    vOut.close(); //close the file
      ////////////////////////////////////////////////////////////////////////////////////////////////////////
      
    cout << "V terminal ="<<vterminal <<" Max height ="<<  maxheight << " Total time= "<< totaltime << endl; // Out put the result and indicates the calculation is finished.
    
    
 return 0;
}









































