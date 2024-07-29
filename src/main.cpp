# include <stdio.h>
# include <unistd.h>
#include <fcntl.h>

# include <iostream>

# include "geometry.hpp"




void render()
{
    //Standard render setup
    int nx = 200;
    int ny = 100;
    int r = 0;
    int g = 255;
    int b = 10;


    // Output File setup 
    int img_fd = open("out/test_img_new.ppm", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (img_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Use std out to write the file, then restore its reference to the console
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout < 0) {
        perror("dup");
        close(img_fd);
        exit(EXIT_FAILURE);
    }

    if (dup2(img_fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(img_fd);
        exit(EXIT_FAILURE);
    }

    // RENDER LOOP
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int y = 0; y < ny; y++) {
        for (int x = 0; x < nx; x++) {
            printf("%d %d %d\n", r, g, b);
        }
    }


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

    
    printf("This should just show up!\n");

    
}

int main() {
    render();

    return 0;
}