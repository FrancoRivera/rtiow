#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r){
	vec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius*radius;

	// getting discriminant to know if it touches the sphere or not
	// otherwise we would need to solve it and stuff this is nicer
	auto discriminant = b*b - 4 *a*c;
	return (discriminant > 0);
}

// get the color of the ray
color ray_color(const ray &r){
	if (hit_sphere(point3(0,0,-1), 0.5, r))
		return color(1,0,0);

	vec3 unit_direction = unit_vector(r.direction());

	auto opacity = 0.5;
	auto t = (unit_direction.y() + 1.0) * opacity;
	// blended value = (1-t) * startValue + t * endValue;
	return (1.0-t)*color(1.0, 1.0,1.0) + t * color (0.5, 0.7, 1.0);
}

int main(){
	// image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	// this makes a vertical window
	const int image_height = image_width / aspect_ratio;

	// camera
	auto viewport_height = 2.0;
	auto viewport_width = aspect_ratio * viewport_height;
	auto focal_length = 1.0;

	auto origin = point3(0,0,0);
	auto horizontal = vec3(viewport_width, 0, 0);
	auto vertical = vec3(0, viewport_height, 0);

	// get the lower left corner from the origin, as a point, we subtract from focal length to
	// push it backwards (further from us)  in Z
	auto lower_left_cornet = origin - horizontal/2 -vertical/2 - vec3(0,0, focal_length);

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
	for (int j = image_height-1; j >= 0; j--){
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for(int i = 0; i < image_width; i++){
			auto u = double(i) / (image_width-1);
			auto v = double(j) / (image_height-1);
			// origin and direction
			ray r(origin, lower_left_cornet + u*horizontal + v*vertical - origin);
			color pixel_color = ray_color(r);
			write_color(std::cout, pixel_color);
		}
	}
	std::cerr << "\nDone.\n";
}
