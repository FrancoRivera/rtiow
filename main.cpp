#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"
#include "material.h"

#include <iostream>

// get the color of the ray
color ray_color(const ray &r, const hittable &world, int depth) {
  hit_record rec;

  // return black if were at the depth limit
  if (depth <= 0){
	  return color(0,0,0);
  }
  if (world.hit(r, 0.001, infinity, rec)) {
	  ray scattered;
	  color attenuation;
	  if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		  return attenuation * ray_color(scattered, world, depth - 1);
	  return color (0,0,0);
  }
  vec3 unit_direction = unit_vector(r.direction());

  auto opacity = 0.5;
  auto t = (unit_direction.y() + 1.0) * opacity;
  // blended value = (1-t) * startValue + t * endValue;
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // image
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400;
  const int image_height = image_width / aspect_ratio;
  const int samples_per_pixel = 5; // for AA
  // be caureful this number basically makes the render X times longer, as it is per pixel,
  // sample: 1, makes it really jagged
  // sample: 10, image looks noisy, dark spots are present, depth makes it a bit less noisy
  // sample: 50, image looks somewhat noisy but better than 10 (at 6 depth, but at depth 20 is perfect)
  // sample: 100 image looks good enough, anything beyond is too much
  const int max_depth = 20; // depth cant be too large as it could blow the stack (recursion)
 // depth 1 makes everything black
 // depth 2 allows for small shdows but no dialectric properties (refraction, reflection)
 // depth 3 makes some reflection
 // depth 6 makes good enough reflection
 // depth 10 looks good
 // depth 50 looks great? - havent test it much

  // World
  hittable_list world;
  auto R = cos(pi/4);

  auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
  auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
  auto material_left = make_shared<dialectric>(1.5);
  auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100.0, material_ground));
  world.add(make_shared<sphere>(point3(0, 0.0, -1), 0.5, material_center));
  world.add(make_shared<sphere>(point3(-1, 0.0, -1), 0.5, material_left));
  world.add(make_shared<sphere>(point3(-1, 0.0, -1), -0.4, material_left));

  world.add(make_shared<sphere>(point3(1, 0.0, -1), 0.5, material_right));

  // Camera
  camera cam(120.0, aspect_ratio);
  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; j--) {
	std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
	for (int i = 0; i < image_width; i++) {
	  color pixel_color(0, 0, 0);
	  for (int s = 0; s < samples_per_pixel; ++s) {
		auto u = (i + random_double()) / (image_width - 1);
		auto v = (j + random_double()) / (image_height - 1);
		ray r = cam.get_ray(u, v);
		pixel_color += ray_color(r, world, max_depth);
	  }
	  write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }
  std::cerr << "\nDone.\n";
}
