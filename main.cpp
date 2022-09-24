#include <iostream>
#include "vec3.h"

int main(){
	const int image_width = 256;
	const int image_height = 256;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	for (int j = image_height-1; j >= 0; j--){
		std::cerr << "\rScanlines reamining: " << j << ' ' << std::flush;
		for(int i = 0; i < image_width; i++){
			vec3 col(double(i) / double(image_height), double(j) / double(image_width), 0.25);
			int ir = static_cast<int>(255.999*col[0]);
			int ig = static_cast<int>(255.999*col[1]);
			int ib = static_cast<int>(255.999*col[2]);

			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	std::cerr << "\nDone.\n";
}
