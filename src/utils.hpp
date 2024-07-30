#ifndef UTILS
#define UTILS

# include <unistd.h>
# include <fcntl.h>

# include <iostream>

const double pi = 3.1415926535897932385;


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


inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

#endif