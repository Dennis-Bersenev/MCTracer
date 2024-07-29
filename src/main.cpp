# include <stdio.h>
# include <unistd.h>
#include <fcntl.h>

# include <iostream>

# include "geometry.hpp"


/*
IN:
    filepath = filepath where the rendered image will be saved.
    saved_stdout = memory location to store the original standard out file descriptor.
OUT:
    the file descriptor for the out image and saved_stdout saved in the passed in address.
*/
int overwrite_sdout(const char * filepath, int * saved_stdout_ptr) {
    // Output File setup 
    int img_fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (img_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // will need this for later
    int temp_stdout = dup(STDOUT_FILENO);
    if (temp_stdout < 0) {
        perror("dup");
        close(img_fd);
        exit(EXIT_FAILURE);
    }
    *saved_stdout_ptr = temp_stdout;

    // overwrite stdout with img path
    if (dup2(img_fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(img_fd);
        exit(EXIT_FAILURE);
    }

    return img_fd;
}


/*
IN:
    img_fd = img_fd for the rendered image.
    saved_stdout_ptr = memory location where original standard out file descriptor is saved.
OUT:
    stdout restored.
*/
void restore_sdout(int img_fd, int * saved_stdout_ptr) {
    int saved_stdout = *saved_stdout_ptr;
    
    // Output file handling/cleanup
    if (fflush(stdout) != 0)
        exit(errno);
    
    close(img_fd);
    
    // Restore the original stdout file descriptor
    if (dup2(saved_stdout, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(saved_stdout);
        exit(EXIT_FAILURE);
    }

    close(saved_stdout);

}


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
    int g = 0;
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