# include <stdio.h>
# include <memory>
# include <limits>
# include <thread>
# include <mutex>
# include <chrono>

# include "utils.hpp"
# include "geometry.hpp"
# include "camera.hpp"
# include "sphere.hpp"
# include "world.hpp"

// std::mutex mu;

/**
* Recursive function responsible for producing final colour of each sample, at each step attenuating reflected colours.
* @param r - initially the sample ray through the pixel whose final colour is to be computed, subsequent calls being invoked
*            on scattered rays from valid intersections.
* @param world - container for all the objects in the scene.
* @param depth - specifies the ray depth; how many times the ray has bounced about the scene.
* @return - final colour of the sample.
*/
vec3 colour(const ray& r, const world& scene)
{
    vec3 res = vec3(0, 0, 0);
    hit_record rec;
    
    if (scene.hit(r, 0.0001, std::numeric_limits<float>::max(), rec))
    {
        // Using sphere norm to colour the ray!
        vec3 unit_norm = unit_vector(rec.normal);
        vec3 norm_remap = vec3((unit_norm.x() + 1.0)/2.0, (unit_norm.y() + 1.0)/2.0, (unit_norm.z() + 1.0)/2.0);
        vec3 norm_col = 255 * norm_remap;

        res += norm_col; 
    }
    else {
        //Background colour
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return 255*((1.0-a)*vec3(1.0, 1.0, 1.0) + a*vec3(0.5, 0.7, 1.0));
        
    }
    return res;
}


/**
* Recursive function responsible for producing final colour of each pixel.
* @param ns - num of samples per pixel.
* @param i - horizontal pixel coord.
* @param j - vertical pixel coord.
* @param nx - img width.
* @param ny - img height.
* @param cam - camera from which to render the scene.
# @param scene - programmatic description of the scene.
* @return - final colour of the sample.
*/
void shade_pixel(int ns, int i, int j, int nx, int ny, camera * cam, const world& scene) {
    vec3 col = vec3(0, 0, 0);
    double u, v;
    ray r;
    for (int s = 0; s < ns; s++)
    {
        u = double(i + random_double()) / double(nx);
        v = double(j + random_double()) / double(ny);

        r = cam->get_ray(u, v);
        col += colour(r, scene);
        
    }
    col /= ns;

    std::cout << col.r() << " " << col.g() << " " << col.b() << "\n";
}


/*
IN:
    nx = img width 
    ny = img height
    ns = num samples per pixel
    obj = reference to the world to be rendered
OUT:
    the image given by fd post-render.
*/
void render(int nx, int ny, int ns, const world& scene)
{
    //Cam setup
    vec3 lookfrom(0, 0, 2);
    vec3 lookat(0, 0, -1);

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 45, double(nx) / double(ny));
    

    // RENDER LOOP
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    std::vector<std::thread> threads;
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            shade_pixel(ns, i, j, nx, ny, &cam, scene);
        }
    }
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

}

int main() {
    // timing execution
    auto start = std::chrono::high_resolution_clock::now();
    
    
    int sd_out;
    int img_fd = overwrite_sdout("out/test_img.ppm", &sd_out);

    // Scene setup
    world sphere_world;
    sphere_world.add(std::make_shared<sphere>(vec3(0,0,-1), 0.5));
    sphere_world.add(std::make_shared<sphere>(vec3(0,-100.5,-1), 100));

    // Rendering pass
    render(720, 640, 20, sphere_world);

    restore_sdout(img_fd, &sd_out);

    auto end = std::chrono::high_resolution_clock::now();

    // Logging exec time
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken to render: " << duration.count() << " seconds" << std::endl;


    return 0;
}