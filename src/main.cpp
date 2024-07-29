# include <stdio.h>

# include "utils.hpp"
# include "geometry.hpp"

/*
IN:
    nx = img width 
    ny = img height
    ns = num samples per pixel
    img_fd = file descriptor of the rendered image
OUT:
    the image given by fd post-render.
*/
void render(int nx, int ny, int ns)
{
    //Standard render setup
    int r = 0;
    int g = 255;
    int b = 255;

    // RENDER LOOP
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int y = 0; y < ny; y++) {
        for (int x = 0; x < nx; x++) {
            printf("%d %d %d\n", r, g, b);
        }
    }
}

int main() {
    
    int sd_out;
    int img_fd = overwrite_sdout("out/test_img_new.ppm", &sd_out);

    render(200, 100, 20);

    restore_sdout(img_fd, &sd_out);

    printf("This should just show up!\n");

    return 0;
}