#include <iostream> 
#include <stdio.h>

int seed = 638597;

long random() {
	seed ^= seed << 5;
	seed ^= seed >> 6;
	seed ^= seed >> 7;
	seed ^= seed << 8;
	return ((long)seed)/(4294967296.0);
}

int main(){
	while (1){
		std::cout << random() << std::endl;
		_sleep(1000);
	}
}
