#include <iostream>
#include <cmath>

float eps = 1e-2;

int fct(int n){
    
    int val=1;
    for (int i=1; i<=n; i++){
        val*=i;
    }
    return val;
}

float bFct(int n, int k){
    return fct(n)/(fct(k)*fct(n-k));
}

float funct(float x){
    return pow(x,4);
}

float trapInt(float lBound, float rBound, float bins){
    
    float lFunc = funct(lBound);
    float rFunc = funct(rBound);
    float dx = (rBound-lBound)/bins;

    float cFactor = (lFunc+rFunc)/2;

    float sumFactor = 0;
    for (int k=1; k<bins; k++){
        sumFactor += funct(lBound + k*dx);
    }

    return dx*(cFactor + sumFactor);
}

float basicDer(float x0){
    return (funct(x0+eps)-funct(x0-eps))/(2*eps);
}

double nthDer(double x0, int n){
    float eps1 = pow(2.2e-16,1.0/(n+1));
    double val=0;
    for (int k=0; k<=n; k++){
        val += pow(-1,n-k) * bFct(n, k) * funct(x0+k*eps1)/pow(eps1,n);
    }
    return val;
}

int main(){

    std::cout << trapInt(0,5,100) << std::endl;
    std::cout << basicDer(10) << std::endl;
    std::cout << nthDer(10,2) << std::endl;

    return 0;
}
