#include <iostream>
#include <climits>
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <stdint.h>
#include "bitmap_image.hpp"

using namespace std;

/// Set your CPU's L1 data cache size (in bytes) here
const int L1D_CACHE_SIZE = 32000;

const int64_t NUMBERS = 1000000;

// FUNCTIONS CONSTANTS
const int EULER_MAX = 1000;		// 1-41
const int LEGENDRE_MAX = 1000;	// 0-16

int64_t* Custom(){

	int64_t *matrix;
	matrix = new int64_t[EULER_MAX];	
	int64_t a = 1;
	int64_t b = 4;
	double c = 4;

	// (b*pow(i,2.0)) + (c*i) - 397; -> Linia con muchos primos

	for(int64_t i = 0; i < EULER_MAX; i++){
		matrix[i] = (b*pow(i,2.0)) + (c*i) - 397;
		//matrix[i] = (a*pow(i,3.0)) + (b*pow(i,2.0)) + (c*i) + 1;
	}

	return matrix;

}
int64_t* Custom2(){

	int64_t *matrix;
	matrix = new int64_t[EULER_MAX];
	int64_t a = 1;
	int64_t b = 1;

	for(int i = 0; i < EULER_MAX; i++){
		matrix[i] = (a*pow(i,2.0)) + i + 1;
	}

	return matrix;

}
int64_t* Legendre(){

	int64_t *matrix;
	matrix = new int64_t[LEGENDRE_MAX];

	for(int i = 0; i < LEGENDRE_MAX; i++){
		matrix[i] = pow(i, 2.0) + i + 17;
	}

	return matrix;

}
int64_t* Euler(){

	int64_t *matrix;
	matrix = new int64_t[EULER_MAX+1];

	for(int i = 0; i < EULER_MAX+1; i++){
		matrix[i] = pow(i, 2.0) - i + 41;
	}

	return matrix;

}

bool isOnMatrix(int64_t *matrix, int64_t number, int64_t max){

	for(int i = 0; i < max; i++){
		if(matrix[i] == number){
			return true;
		}
	}

	return false;

}
bool esPar(int64_t num){
	if(num % 2 == 0)
		return true;
	else
		return false;
}
bool esPrimo(int64_t i){

	int64_t j;
	int64_t t;

	if(i == 1 || i == 2)
		return false;

	t = true;

	for(j = 2; j < i-1; j++){
		if(i % j == 0){
			t = false;
			j = i;
		}
	}

	if(t > 0)
		return true;
	else
		return false;

}

void main()
{
	//comparison_test();
	//sum_test();
	//minus_test();
	//multiply_test();
	//division_test();
	/*// Central point
	draw.pen_color(255,0,255);
	draw.dot(ini_x,ini_y);*/

	bool salir = false;
	int64_t numero = NUMBERS;

	time_t inicio;
	time_t final;

	int64_t min_prime = 3;
	int64_t max_prime = 0;
	int64_t wh = 0;
	int64_t comp = 1;
	int64_t sum = 1;
	bool salir2 = false;
	bool salir3 = false;
	int64_t offset_x = 0;
	int64_t offset_y = 0;
	int64_t dir = 0;
	int64_t image_width = 0;
	int64_t imatge_height = 0;

	int64_t *custom = Custom();
	int64_t *euler = Euler();
	int64_t *legendre = Legendre();	

	cout << endl << "Calculating image dimensions..." << endl;

	do{
		if (numero >= comp && numero < (comp + sum)){
			wh += 1;
			salir3 = true;
		} else {
			wh += 1;
			comp += sum;
			sum += 2;
		}
	} while(!salir3);

	if(esPar(wh)){
		offset_x = (wh/2)-1;
		offset_y = (wh/2);
	} else {
		offset_y = (wh-1)/2;
		offset_x = offset_y;
	}

	int64_t ini_x = offset_x;
	int64_t ini_y = offset_y;

	cout << "Creating an " << wh << "x" << wh << " Bitmap..." << endl;	

	bitmap_image img(wh,wh);

	img.set_all_channels(0,0,0);

	image_drawer draw(img);
	draw.pen_width(1);
	draw.pen_color(0,255,0);

	int64_t cont = 0;
	int64_t cont_2_veces = 0;
	int64_t times = 1;
	int64_t que_porcentaje_hemos_superado = -10;
	bool printar_porcentaje = false;

	cout << "Applying sieve..." << endl;

	time(&inicio);

	int sqrtt = (int) sqrt((double) numero);
	int64_t count = (numero < 2) ? 0 : 1;
	int64_t s = 2;
	int64_t n = 1;

	// vector used for sieving
	vector<char> sieve(L1D_CACHE_SIZE);

	// generate small primes <= sqrt
	vector<char> is_prime(sqrtt + 1, 1);
	for (int64_t i = 2; i * i <= sqrtt; i++)
		if (is_prime[i])
			for (int64_t j = i * i; j <= sqrtt; j += i)
			is_prime[j] = 0;

	vector<int> primes;
	vector<int> next;

	for (int64_t low = 0; low <= numero; low += L1D_CACHE_SIZE)
	{
		fill(sieve.begin(), sieve.end(), 1);

		// current segment = interval [low, high]
		int64_t high = min(low + L1D_CACHE_SIZE - 1, numero);

		// store small primes needed to cross off multiples
		for (; s * s <= high; s++)
		{
			if (is_prime[s])
			{
				primes.push_back((int) s);
				next.push_back((int)(s * s - low));
			}
		}
		// sieve the current segment
		for (size_t i = 1; i < primes.size(); i++)
		{
			int j = next[i];
			for (int k = primes[i] * 2; j < L1D_CACHE_SIZE; j += k)
				sieve[j] = 0;
			next[i] = j - L1D_CACHE_SIZE;
		}

		for (; n <= high; n += 1){

			//cout << n << endl;

			//double percen = ((double)n / (double)numero) * 100;
			//int percen_int = floor(percen);

			/*if(percen_int > que_porcentaje_hemos_superado){
				que_porcentaje_hemos_superado = percen_int;
				printar_porcentaje = true;}

			if(printar_porcentaje){
				//cout << que_porcentaje_hemos_superado << "%" << endl;
				printar_porcentaje = false;}
				*/

			if(isOnMatrix(custom,n,EULER_MAX)){
				//draw.pen_color(255,255,255);	// Blanco
				//draw.dot(offset_x,offset_y);
				//draw.pen_color(0,255,0);
			} else if(isOnMatrix(euler,n,EULER_MAX)){
				//draw.pen_color(255,0,255);
				//draw.dot(offset_x,offset_y);
				//draw.pen_color(0,255,0);
			} else {
				//draw.dot(offset_x,offset_y);
			}
			
			if(n % 2 != 0)
			{
				// If it's prime
				if (sieve[n - low])
				{		  
				  count++;
				  
				  if(isOnMatrix(custom,n,EULER_MAX)){
					  //draw.pen_color(255,0,0);	// Rojo
					  //draw.dot(offset_x,offset_y);
					  //draw.pen_color(0,255,0);
				  } else if(isOnMatrix(euler,n,EULER_MAX)){
					  //draw.pen_color(0,0,255);
					  //draw.dot(offset_x,offset_y);
					  //draw.pen_color(0,255,0);
				  } else {
					  draw.dot(offset_x,offset_y);
				  }
				}
			}

			if(cont == times)
			{
				if(cont_2_veces == 0){
					cont_2_veces += 1;
				} else if(cont_2_veces == 1){
					times += 1;
					cont_2_veces = 0;
				}
				dir += 1;
				cont = 0;
			}
			cont += 1;

			dir = dir % 4;

			switch(dir)
			{
			case 0:
				offset_x += 1;
				break;
			case 1:
				offset_y -= 1;
				break;
			case 2:
				offset_x -= 1;
				break;
			case 3:
				offset_y += 1;
				break;
			}

		}
	}

	delete []custom;
	delete []euler;
	delete []legendre;	

	time(&final);

	cout << "----------------------------" << endl;
	cout << "Timestamp: " << difftime(final,inicio) << " seconds" << endl;

	cout << count << " primes found." << endl;

	img.save_image("Images/ulam.bmp");
}