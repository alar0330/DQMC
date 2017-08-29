#include <iostream>

double eval(double (*fun)(double), double z);
double square(double x);
double cube(double x);

using namespace std;

int main() {

  double (*ptrSq)(double) = square;
  double (*ptrCu)(double) = cube;

  cout << "eval of square: " << eval(ptrSq, 2) << endl;
  cout << "eval of cube: " << eval(ptrCu, 3) << endl;

  return 0;

}

double eval(double (*fun)(double), double z) {
  return fun(z)+1;
}

double square(double x) {
  return x*x;
}

double cube(double x) {
  return x*x*x;
}
