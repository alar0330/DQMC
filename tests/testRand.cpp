// TEST

#include <iostream>
#include <random>
#include <chrono>

using namespace std;

int main()
{
    // Random seed
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    // Initialize Mersenne Twister pseudo-random number generator
    mt19937 gen(seed);

    // Generate pseudo-random numbers
    // uniformly distributed in range (1, 100)
    normal_distribution<double> dis(0.0, 0.1);
	uniform_real_distribution<> rndreal(0,1);

    // Generate ten pseudo-random numbers
    for (int i = 0; i < 10; i++)
    {
        double randomND = dis(gen);
        cout << "\nRandom ND = " << randomND;
    }
	
	for (int i = 0; i < 10; i++)
    {
        double randomR = rndreal(gen);
        cout << "\nRandom R = " << randomR;
    }
}