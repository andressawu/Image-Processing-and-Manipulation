#include <stdio.h>
#include <stdlib.h>

// Structure to hold grayscale pixel data
typedef struct {
    unsigned char value;
} PixelGray;

// Function to read a PGM image into a 2D array
PixelGray** readPGM(const char* filename, int* width, int* height) {
    FILE *myfile = fopen(filename, "rb");
    if (myfile == NULL) {
        perror("Error: file not open");
        exit(1);
    }

    // Read PGM header
    char magic[3];
    if (fscanf(myfile, "%2s", magic) != 1 || magic[0] != 'P' || magic[1] != '5') {
        fprintf(stderr, "wrong PGM form\n");
        exit(1);
    }

    // Read width and height
    if (fscanf(myfile, "%d %d", width, height) != 2) {
        fprintf(stderr, "Error: reading width and height\n");
        exit(1);
    }

    // Read max value (should be 255)
    int maxVal;
    if (fscanf(myfile, "%d", &maxVal) != 1 || maxVal != 255) {
        fprintf(stderr, "wrong max value\n");
        exit(1);
    }

    // Allocate memory for image
    PixelGray** img = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    for (int i = 0; i < *height; ++i) {
        img[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
    }

    // Consume the newline character
    fgetc(myfile);

    // Read image data

    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            fread(&img[i][j].value,sizeof(PixelGray),1, myfile);
        }
    }

    fclose(myfile);
    return img;
}

// Function to write a 2D matrix as a PGM image
void writePGM(const char* filename, PixelGray** matrix,const int* width,const int* height) {
    FILE* myfile = fopen(filename, "wb");
    if (myfile == NULL) {
        perror("Error: file not open");
        exit(1);
    }

    // Write PGM header
    fprintf(myfile, "P5\n%d %d\n%d\n", *width, *height, 255);

    // Write image data
    for (int i = 0; i < *height; ++i) {
        for (int j = 0; j < *width; ++j) {
            fwrite(&matrix[i][j].value, sizeof(PixelGray),1, myfile);
        }
    }

    fclose(myfile);
}

// Function to threshold the image matrix
PixelGray** threshold(PixelGray** matrix,const int* width,const int* height){
    PixelGray** thresh = (PixelGray**)malloc(*height * sizeof(PixelGray*));
    for (int i = 0; i < *height; ++i) {
        thresh[i] = (PixelGray*)malloc(*width * sizeof(PixelGray));
        for (int j = 0; j < *width; ++j) {
            if (matrix[i][j].value > 80) {
                thresh[i][j].value = 255;
            } else {
                thresh[i][j].value = 0;
            }
        }
    }
    return thresh;
}

// Function to rotate the image matrix
PixelGray** rotate(PixelGray** matrix, int* width, int* height){
    PixelGray** rotate = (PixelGray**)malloc(*width * sizeof(PixelGray*));
    for (int i = 0; i < *width; ++i) {
        rotate[i] = (PixelGray*)malloc(*height * sizeof(PixelGray));
        for (int j = 0; j < *height; ++j) {
            rotate[i][j].value = matrix[j][i].value;
        }
    }

    return rotate;
}

//main function - DO NOT MODIFY
int main() {
    int width, height;  // variable to hold width and height. Use reference in other functions

    PixelGray** image_original = readPGM("lenna.pgm", &width, &height);
    // Now you have the grayscale image data in the 'image_original' 2D array

    // Access pixel data using image[row][col].value
    // For example, to access the pixel at row=2, col=3:
    // unsigned char pixel_value = image[2][3].value;

    // Create a new 2D array 'image_thresh' to store the threshold image
    PixelGray** image_thresh = threshold(image_original, &width, &height);
    //write the image data as "threshold.pgm"
    writePGM("threshold.pgm", image_thresh, &width, &height);

    // Create a new 2D array 'image_rotate' to store the rotated image
    PixelGray** image_rotate = rotate(image_original, &width, &height);
    //write the image data as "rotate.pgm"
    writePGM("rotate.pgm", image_rotate, &width, &height);

    image_rotate = rotate(image_rotate, &width, &height);
    //write the image data as "rotate_again.pgm"
    writePGM("rotate_again.pgm", image_rotate, &width, &height);

    // Free the allocated memory when you're done
    for (int i = 0; i < height; ++i) {
        free(image_original[i]);
        free(image_thresh[i]);
        free(image_rotate[i]);
    }
    free(image_original);
    free(image_thresh);
    free(image_rotate);
    return 0;
}