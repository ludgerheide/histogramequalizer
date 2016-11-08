//
//  main.c
//  histogramequalizer
//
//  Created by Ludger Heide on 11.10.16.
//  Copyright © 2016 Ludger Heide. All rights reserved.
//

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "histogramequalizer.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    
    if(argc != 3) {
        printf("Usage: histogramequalizer infile outfile\n");
        return 1;
    }
    
    const char* infile = argv[1];
    const char* outfile = argv[2];
    
    int x,y,n;
    unsigned char *data = stbi_load(infile, &x, &y, &n, 0);
    
    unsigned char* inputImage;
    
    //Create grayscale if we don't have it
    if(n == 3) {
        const double red_multiplier = 0.33;
        const double green_multiplier = 0.33;
        const double blue_multiplier = 0.33;
        inputImage = malloc(sizeof(unsigned char) * x * y);
        
        for(unsigned long i = 0; i < x*y; i++) {
            inputImage[i] = round(red_multiplier*data[3*i] + green_multiplier*data[3*i+1] + blue_multiplier*data[3*i+2]);
        }
        free(data);
    } else if(n == 1) {
        inputImage = data;
    } else {
        printf("Unsupported number of channels (%i)!", n);
        return  1;
    }
    
    //Debug: write the grayscale image
    //stbi_write_png("/tmp/bw.png", x, y, 1, inputImage, x);
    
    unsigned char* outputImage = malloc(sizeof(unsigned char) * x * y);
    if(outputImage == NULL) {
        printf("malloc error!");
        return 1;
    }
    
    equalizehistogram(x, y, inputImage, outputImage);
    
    
    stbi_write_png(outfile, x, y, 1, outputImage, x);
    
    return 0;
}
