#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "sphere.h"

#include <iostream>

// get the color of the ray
color ray_color(const ray &r, const hittable &world, int depth) {
  hit_record rec;

  // return black if were at the depth limit
  if (depth <= 0){
	  return color(0,0,0);
  }
  if (world.hit(r, 0, infinity, rec)) {
	  point3 target = rec.p + rec.normal + random_in_unit_sphere();
	return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
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
  const int samples_per_pixel = 100; // for AA
  const int max_depth = 10;

  // World
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Camera
  camera cam;
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
