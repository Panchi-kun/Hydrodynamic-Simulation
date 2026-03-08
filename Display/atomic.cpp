#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
using namespace std;

const float e  = 2.718281;
const float pi = 3.141592;
const float a0 = 5.291772e-11;

int fct(int n){
  return (n == 1 || n == 0) ? 1 : fct(n - 1) * n;
}

float legendre(int l, int m, float theta){
    float termPre = pow(0.5*sin(theta),abs(m));
    
    float termSummation = 0;
    for (int k=abs(m); k<=l; k++){
        float termFacto = fct(l+k)/(fct(k)*fct(l-k)) * 1/(fct(k-abs(m)));
        float termCos   = pow(0.5*(cos(theta)-1),k-abs(m));
        termSummation  += termFacto * termCos;
    }

    return termPre * termSummation;
}

vector<float> sphericalSep(int l, int m, float theta, float phi){

    float termConst  = pow(-1,abs(m)) * sqrt((2*l+1)/(4*pi) * fct(l-abs(m))/fct(l+abs(m)));
    float termLeg    = legendre(l, abs(m), theta);

    float termReal   = termConst * termLeg * cos(abs(m)*phi);
    float termComplx = termConst * termLeg * sin(abs(m)*phi);

    if (m<0){
        termReal   = pow(-1,abs(m)) *   termReal;
        termComplx = pow(-1,abs(m)) * (-termComplx); 
    }
    
    vector<float> comps;
    comps.push_back(termReal);
    comps.push_back(termComplx);
    
    return comps;
}

float laguerre(int n, int l, float r) {
    float value = 0;
    for (int k = 0; k <= (n-l-1); k++) {
        value += (k % 2 == 0 ? 1 : -1)
                 * fct(n+l) / (fct(n-l-1-k) * fct(2*l+1+k) * fct(k))
                 * pow((2.0*r)/(n*a0), k);
    }
    return value;
}

float radialSep(int n, int l, float r){
    float termConst   = -pow(2/(n*a0),1.5) * sqrt(fct(n-l-1)/(2*n*pow(fct(n+l),3)));
    float radTerm1    =  pow((2*r)/(n*a0),l)*pow(e,-r/(n*a0));
    float radLaguerre =  laguerre(n,l,r);

    return termConst * radTerm1 * radLaguerre;
}

int main(){

    const std::complex<double> i(0.0,1.0);    
    std::cout << i << std::endl;
    return 0;
}