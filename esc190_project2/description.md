# Project 2: Seam Carving

Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of width (or height) at a time. A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row; a horizontal seam is a path of pixels connected from the left to the right with one pixel in each column. Below left is the original 505-by-287 pixel image; below right is the result after removing 150 vertical seams, resulting in a 30% narrower image. Unlike standard content-agnostic resizing techniques (such as cropping and scaling), seam carving preserves the most interest features (aspect ratio, set of objects present, etc.) of the image. Although the underlying algorithm is simple and elegant, it was not discovered until 2007. Now, it is now a core feature in Adobe Photoshop and other computer graphics applications.

In this assignment, you will create a data type that resizes a H-by-W image using the seam-carving technique.You will first compute the dual-gradient energy function, and then find vertical “seams” – paths from the top to the bottom of the image – such that the sum of the dual-gradient energy values in the pixels along the path is as small as possible

<i>Notation</i>. In image processing, pixel (y, x) refers to the pixel in column x and row y, with pixel (0,0) at the upper-left corner and pixel (H-1, W-1) at the lower-right corner.

<i>Energy calculation.</i> The first step is to calculate the energy of a pixel, which is a measure of its perceptual importance - the higher the energy, the less likely that the pixel will be included as part of a seam.

## Part 1: Dual-Gradient Energy Function
In this part, you will write the function ```void calc_energy(struct rgb_img *im, struct rgb_img **grad);```<be>
The function will compute the dual-gradient energy function, and place it in the ```struct rbg_img``` ```*grad```.

The energy of pixel $`(y,x)`$ is $`\sqrt{\Delta_x^2(y,x) + \Delta_y^2(y,x)}`$. Here,

$`\Delta_x^2(y,x) = R_x(y,x)^2 + G_x(y,x)^2 + B_x(y,x)^2`$ <br>
$`\Delta_y^2(y,x) = R_y(y,x)^2 + G_y(y,x)^2 + B_y(y,x)^2`$

$`R_x, R_y, …, B_y`$ are the differences in the red, green, and blue components of pixels surrounding the central pixel, along the x and y-axis. For example,

$`R_x(y,x) = (y,x+1)_{red} - (y,x-1)_{red}`$

For pixels at the edge of the image, you should “wrap around” the image.

You are storing the dual-gradient energy in an image. to do that, divide the original energy by 10, and cast it to (```uint8_t```). Fpr each pixel, set the r, g, and b channels to the same value (the energy divided by 10 and cast to ```uint8_t```).

You can print out the dual-gradient energy using
```
struct rgb_img *grad;
calc_energy(im, &grad);
print_grad(grad);
```

## Part 2: Cost Array
Define the function ```dynamic_seam(struct rgb_img *grad, double **best_arr)``` which allocates and computes the dynamic array ```*best_arr```.<br>
```(*best_arr)[i*width+j]``` contains the minimum cost of a seam from the top of grad to the point $`(i,j)`$.

## Part 3: Recover the seam
Write a function ```void recover_path(double *best, int height, int width, int **path);```<br>
This function allocates a path through the minimum seam as defined by the array ```best_arr```.

## Part 4: Write a function that removes the seam
```voidremove_seam(struct rgb_img *src, struct rgb_img **dest, int *path);```<br>
The function creates the destination image, and writes to it the source image, with the seam removed.

## Part 5: Not for submission
Run your program to repeatedly remove seams from an image, and visualize the result.
```
struct rgb_img *im;
struct rgb_img *cur_im;
struct rgb_img *grad;
double *best;
int *path;

read_in_img(&im, "HJoceanSmall.bin");
    
for(int i = 0; i < 5; i++){
    printf("i = %d\n", i);
    calc_energy(im,  &grad);
    dynamic_seam(grad, &best);
    recover_path(best, grad->height, grad->width, &path);
    remove_seam(im, &cur_im, path);

    char filename[200];
    sprintf(filename, "img%d.bin", i);
    write_img(cur_im, filename);


    destroy_image(im);
    destroy_image(grad);
    free(best);
    free(path);
    im = cur_im;
}
destroy_image(im);
```
