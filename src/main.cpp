# include <stdio.h>
# include <memory>
# include <limits>
    
# include "utils.hpp"
# include "geometry.hpp"
# include "camera.hpp"
# include "sphere.hpp"

/**
* Recursive function responsible for producing final colour of each sample, at each step attenuating reflected colours.
* @param r - initially the sample ray through the pixel whose final colour is to be computed, subsequent calls being invoked
*            on scattered rays from valid intersections.
* @param world - container for all the objects in the scene.
* @param depth - specifies the ray depth; how many times the ray has bounced about the scene..
* @return - final colour of the sample.
*/
vec3 colour(const ray& r, geometry * obj, int depth)
{
    hit_record rec;
    
    if (obj->hit(r, 0.0001, std::numeric_limits<float>::max(), rec))
    {
        return vec3(255, 0, 0);   
    }
    //Background colour
    return vec3(0, 255, 0);
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
void render(int nx, int ny, int ns, geometry * obj)
{
    //Standard render setup
    // int r = 0;
    // int g = 255;
    // int b = 255;

    //Cam setup
    vec3 lookfrom(0, 0, 10);
    vec3 lookat(0, 0, -1);

    camera cam(lookfrom, lookat, vec3(0, 1, 0), 45, double(nx) / double(ny));
    

    // RENDER LOOP
    double u, v;
    ray r;
    vec3 col;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            u = double(i) / double(nx);
            v = double(j) / double(ny);

            r = cam.get_ray(u, v);
            col = colour(r, obj, 0);
            std::cout << col.r() << " " << col.g() << " " << col.b() << "\n";
        }
    }

    // for (int y = 0; y < ny; y++) {
    //     for (int x = 0; x < nx; x++) {
    //         printf("%d %d %d\n", r, g, b);
    //     }
    // }
}

int main() {
    
    int sd_out;
    int img_fd = overwrite_sdout("out/test_img_new.ppm", &sd_out);
    geometry * s = new sphere(vec3(0, 0, -1), 1);

    // Rendering pass
    render(200, 100, 20, s);

    restore_sdout(img_fd, &sd_out);

    delete s;

    return 0;
}