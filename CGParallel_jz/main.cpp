#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

#include <mpi.h>
//#include <boost/timer/timer.hpp>
#define F77NAME(x) x##_

extern "C" {
    
    void F77NAME(dgemm) (const char& trans, const char& transb, const int& m, const int& n,
                         const int& k, const double& alpha, const double *A, const int& lda,
                         const double*B, const int& ldb, const double& beta, const double* C, const int& ldc);
                         
    void F77NAME(daxpy) (const int& N, const double&alpha, const double*X, const int& incx, const double* Y, const int& incy);
    
    void F77NAME(dsyrk)(const char& uplo, const char&trans, const int&n, const int&k, const double&alpha, const double *A,
                                                  const int&lda, const double &beta,  const double *C, const int& ldc);
    
    void F77NAME(dgemv)(const char& trans, const int&N, const int&M, const double& alpha, const double*A, const int&lda,
                                                    const double*x, const int&incx, const double&beta, const double*y, const int& incy);
                                                    
    double F77NAME(dasum)(const int &N, const double *x, const int &incx);
    double F77NAME(ddot)(const int& N, const double *x, const int&incx, const double *y, const int&incy);
    void F77NAME(dscal)(const int&N, const double &alpha, const double *x, const int &incx);
    double F77NAME(dnrm2)(const int &N, const double *x, const int &incx);
     
    void F77NAME(dcopy)(const int&N, const double *x, const int &incx, const double*y, const int&incy);

}

void PrintMatrix(int rows, int columns, double* H, int r) {
    cout.precision(4);
    cout << "Rank" << r << "Receives matrix"<< endl;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            cout << "    " << H[j*rows+i] << "    ";
        }
        cout << endl;
    }
    cout << endl;
}

void PrintVector(int n, double* u) {
    for (int i = 0; i < n; ++i) {
        cout << u[i] << " ";
    }
    cout << endl;
    cout << "\n" << endl;
}

double rdgenerator(){
    double output = 0; 
    double range = 1;
    
     double detem = double(rand())/RAND_MAX * range;
     if (detem> 0.5){
         output = double(rand())/RAND_MAX * range;
         
     }
     else{
         output = -double(rand())/RAND_MAX * range;
         }
         
    return output;
}

double *symmatrice(const int N){
    
    double *symmatrice = new double [N*N];
    double entry = 0;
    
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            if (i == j){
                 symmatrice[j*N+i] = rdgenerator();
            }
            else{
                entry = rdgenerator();
                symmatrice[i*N+j] = entry;
                symmatrice[j*N+i] = entry;
            }
        }
    }
    return symmatrice;
}

int main(int argc, char *argv[])
{       
    //boost::timer::auto_cpu_timer t;
    int nsv = 8;
    double *A = new double [nsv*nsv];
    double *C = new double [nsv*nsv];
    double *x = new double [nsv];
    double *b = new double [nsv];
    double *Ct = new double [nsv*nsv];
    double limit;
    //double *prod = new double;   
   
    int s,r;
    int input;
    int err = MPI_Init(&argc, &argv);
    if (err != MPI_SUCCESS){
        std::cout << "Failed to initialise MPI" << std::endl;
        MPI_Finalize();
        return 1;
    }
    
    MPI_Comm_rank(MPI_COMM_WORLD, &r); 
    MPI_Comm_size(MPI_COMM_WORLD, &s);
    double *cc = new double[nsv*nsv/s];
    double *bb = new double[nsv/s];
    double *rf = new double[nsv/s];
    double *rftemp = new double[nsv/s];
    double *rr = new double[nsv];
    double *pp = new double[nsv/s];
    double *pf = new double[nsv];
    double *ap = new double[nsv];
    double *ax = new double[nsv/s];
    double *xf = new double[nsv];
    double *ap_loc = new double[nsv/s];
    double sum_loc;
    double aklow, akup, ak, akup_loc, bk, bkup, bklow=0.1, bkup_loc, bklow_loc;
    int k=0;
    if (r==0){
        srand(time(0));
        A = symmatrice(nsv);
                
                
        for (int i=0; i<nsv; i++){
            x[i] = (double)rand()/RAND_MAX;
        }
        
        cout << " The matrix A is " << endl;
        PrintMatrix(nsv,nsv, A,999);
        
        F77NAME(dgemm) ('T', 'N', nsv, nsv, nsv, 1, A, nsv, A, nsv, 0, C, nsv);
        cout<<"\n"<<endl;
        cout<<"The positive-definite  product  C = Mt*M is"<<endl;
        for (int i=0;i<nsv;i++){
              for (int j=0;j<nsv;j++){
                  cout<<C[j*nsv +i]<<" ";
         }
         cout << endl;
         }
         
        for (int i=0; i<nsv; i++){
            for (int j=0; j<nsv; j++){
                Ct[i*nsv+j] = C[j*nsv+i];
            }
                }
                
        cout << " The matrix inverse passed in is " << endl; 
        PrintMatrix(nsv,nsv, Ct,999);
         
        F77NAME(dgemv)('N', nsv, nsv, 1.0, C, nsv, x, 1,0.0, b, 1);
        cout<<"\n"<<endl;
        cout<<"The vector b=C*x is"<< endl;
        for (int i=0; i<nsv; i++){
            cout<<b[i]<<" ";
            
        }
        cout<<endl;
        
        cout << " The vector x is " << endl;
        PrintVector(nsv, x);
        
            
    }

    //MPI_Bcast(B, nsv, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, nsv/s, MPI_DOUBLE, bb, nsv/s, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Ct, (nsv*nsv)/s, MPI_DOUBLE, cc, (nsv*nsv)/s, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(Ct, nsv*nsv, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    for (int i=0; i< nsv/s; ++i){
        ax[i] = 0;
        for (int j=0; j< nsv; ++j){
            ax[i] += cc[i*nsv+j]*xf[j];
        }
    }
    
    
    F77NAME(daxpy) (nsv/s, -1, ax, 1, bb, 1);
    
    F77NAME(dcopy)(nsv/s, bb, 1,rf,1);
    F77NAME(dcopy)(nsv/s, rf, 1,pp,1);
    sum_loc = F77NAME(dasum)(nsv/s, rf, 1);
    MPI_Allreduce(&sum_loc, &limit, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    MPI_Allgather(pp, nsv/s, MPI_DOUBLE, pf, nsv/s, MPI_DOUBLE, MPI_COMM_WORLD);
    
    while(abs(limit) >0.0000001){
         
         for (int i=0; i< nsv; ++i){
             ap[i] = 0;
             for (int j=0; j< nsv; ++j){
                ap[i] += Ct[i*nsv+j]*pf[j];
                }
              }  

         
         aklow = F77NAME(ddot)(nsv, pf, 1, ap, 1);
         akup_loc =  F77NAME(ddot)(nsv/s, rf, 1, rf, 1);
         MPI_Allreduce(&akup_loc, &akup, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
         ak = akup/aklow;
         
         F77NAME(daxpy) (nsv, ak, pf, 1, xf, 1);
         F77NAME(dcopy)(nsv/s, rf, 1,rftemp,1);

         for (int i=0; i< nsv/s; ++i){
             ap_loc[i] = 0;
             for (int j=0; j< nsv; ++j){
                ap_loc[i] += cc[i*nsv+j]*pf[j];
                }
              }   
         
         F77NAME(daxpy) (nsv/s, -ak, ap_loc, 1, rf, 1);
         sum_loc = F77NAME(dasum)(nsv/s, rf, 1);
         //cout<<"The sumloc is"<< sum_loc<<endl;
         MPI_Allreduce(&sum_loc, &limit, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
         
         bkup_loc = F77NAME(ddot)(nsv/s, rf, 1, rf, 1);
         bklow_loc = F77NAME(ddot)(nsv/s, rftemp, 1, rftemp, 1);
         
         MPI_Allreduce(&bkup_loc, &bkup, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
         MPI_Allreduce(&bklow_loc, &bklow, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
         
         bk = bkup/bklow;
         
         F77NAME(dscal)(nsv, bk, pf, 1);
         
         MPI_Allgather(rf, nsv/s, MPI_DOUBLE, rr, nsv/s, MPI_DOUBLE, MPI_COMM_WORLD);
         F77NAME(daxpy) (nsv, 1, rr, 1, pf, 1);
         cout << "The " << k <<"th iteration of x is" << endl;
         PrintVector(nsv, xf);
         k++;
              
    }


    
    MPI_Barrier(MPI_COMM_WORLD);
    
    if(r==0){
        //cout << "The number of iterations is " << k << endl;
        cout << "The exact solution of x is" << endl;
        PrintVector(nsv, x);
        
    }
    MPI_Finalize();
	return 0;
}