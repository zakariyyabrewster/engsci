#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "seamcarving.h"


void calc_energy(struct rgb_img *im, struct rgb_img **grad)
{
    create_img(grad, im->height, im->width);

    for (int y = 0; y < im->height; y++){
        for (int x = 0; x < im->width; x++){
            int rx = get_pixel(im, y, (x+1) % im->width, 0) - get_pixel(im, y, (x - 1 + im->width) % im->width, 0);
            int gx = get_pixel(im, y, (x+1) % im->width, 1) - get_pixel(im, y, (x - 1 + im->width) % im->width, 1);
            int bx = get_pixel(im, y, (x+1) % im->width, 2) - get_pixel(im, y, (x - 1 + im->width) % im->width, 2);
            int ry = get_pixel(im, (y+1) % im->height, x, 0) - get_pixel(im, (y - 1 + im->height) % im->height, x, 0);
            int gy = get_pixel(im, (y+1) % im->height, x, 1) - get_pixel(im, (y - 1 + im->height) % im->height, x, 1);
            int by = get_pixel(im, (y+1) % im->height, x, 2) - get_pixel(im, (y - 1 + im->height) % im->height, x, 2);
            int x_grad = rx*rx + gx*gx + bx*bx;
            int y_grad = ry*ry + gy*gy + by*by;
            uint8_t energy = sqrt(x_grad + y_grad) / 10;
            set_pixel(*grad, y, x, energy, energy, energy);
            }
        }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr)
{
    *best_arr = (double *)malloc(grad->width * grad->height * sizeof(double)); // malloc for a 2D array of size width x height
    for (int x = 0; x < grad->width; x++){ 
        (*best_arr)[x] = (double)get_pixel(grad, 0, x, 0); // set the first row of the 2D array to the first row of the gradient image
    }

    for (int y = 1; y < grad->height; y++){ // y = 1 to start at second row
        for (int x = 0; x < grad->width; x++){ // iterate through row from left to right
            double min = (*best_arr)[(y-1)*grad->width + x]; // set min to the value of the pixel above (Ex. if on row 3, min = pixel x in row 2)
            if (x > 0 && (*best_arr)[(y-1)*grad->width + x - 1] < min){ // if the pixel to the left of the current min pixel is less than min
                min = (*best_arr)[(y-1)*grad->width + x - 1]; // set min to the pixel to the left of the current min pixel
            }
            if (x < grad->width - 1 && (*best_arr)[(y-1)*grad->width + x + 1] < min){ // if the pixel to the right of the current min pixel is less than min
                min = (*best_arr)[(y-1)*grad->width + x + 1]; // set min to the pixel to the right of the current min pixel
            }
            (*best_arr)[y*grad->width + x] = (double)get_pixel(grad, y, x, 0) + min; // set pixel in lower row to the value of the pixel + the value of min --> represents min energy path to the pixel
        }
    }
}

void recover_path(double *best, int height, int width, int ** path){
    *path = (int *)malloc(height * sizeof(int)); // initialize mem for path
    double min;
    // find the pixel in the last row with the least energy
    for (int x = 0; x < width; x++){
        if (x == 0){
            min = best[(height - 1) * width]; // set min to the first pixel in the last row
            (*path)[height - 1] = 0; // set the last pixel in the path to 0 before parsing through row
        }
        if (best[(height - 1) * width + x] < min){ // if energy to get to pixel is less than min
            min = best[(height - 1) * width + x]; // set min to energy to get to pixel
            (*path)[height - 1] = x; // set the last pixel in the path to x
        }
    }

    for (int y = height - 2; y >= 0; y--){ // iterate through rows from bottom to top starting at second last row
        int x = (*path)[y + 1]; // set x to the index of the lowest energy pixel in the row below
        int x_new = x; // new var to store new index if changed better pixel found
        double min = best[y * width + x]; // set min to the energy to get to the pixel
        if (x > 0 && best[y*width + x - 1] < min){ // check if pixel to the left of current pixel has less energy
            min = best[y*width + x - 1]; // set min to energy to get to pixel, updates min
            x_new = x - 1; // set new index to the index of the pixel to the left
        }
        if ((*path)[y+1] < width - 1 && best[y*width + (*path)[y+1] + 1] < min){ // check if pixel to the right of current pixel has less energy
            min = best[y*width + (*path)[y+1] + 1]; // updates min
            x_new = x + 1; // set new index to the index of the pixel to the right (if both if statements are true, it will accurately choose the pixel to the right)
        }
        (*path)[y] = x_new; // set the pixel in the path to the new index
    }
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path)
{
    create_img(dest, src->height, src->width - 1);
    for (int y = 0; y < src->height; y++){
        for (int x = 0; x < src->width; x++){
            if (x < path[y]){
                set_pixel(*dest, y, x, get_pixel(src, y, x, 0), get_pixel(src, y, x, 1), get_pixel(src, y, x, 2));
            }
            else if (x > path[y]){
                set_pixel(*dest, y, x-1, get_pixel(src, y, x, 0), get_pixel(src, y, x, 1), get_pixel(src, y, x, 2));
            }
        }
    }
    
}

