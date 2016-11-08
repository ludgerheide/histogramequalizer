//
//  histogramequalizer.c
//  histogramequalizer
//
//  Created by Ludger Heide on 11.10.16.
//  Copyright © 2016 Ludger Heide. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

#define GRAY_LEVELS 256

static void calculateHistogram(unsigned int xSize, unsigned int ySize, unsigned char* inputImage, unsigned long* histogram) {
    memset(histogram, 0, sizeof(unsigned long) * GRAY_LEVELS);
    
    for(unsigned long i = 0; i < (unsigned long) xSize * (unsigned long) ySize; i++) {
        histogram[inputImage[i]]++;
    }
}

static void normalizeHistogram(unsigned long* histogram, double* normalizedHistogram, unsigned int xSize, unsigned int ySize) {
    for(unsigned short i = 0; i < GRAY_LEVELS; i++) {
        normalizedHistogram[i] = (double)histogram[i] / (xSize*ySize);
    }
}

static void cumulateHistogram(double* normalizedHistogram, double* cumulativeHistogram) {
    double total = 0;
    for(unsigned short i = 0; i < GRAY_LEVELS; i++) {
        total += normalizedHistogram[i];
        cumulativeHistogram[i] = total;
    }
}

int equalizehistogram(unsigned int xSize, unsigned int ySize, unsigned char* inputImage, char* outputImage) {
    bool allocFailed = false;
    
    unsigned long* histogram = malloc(sizeof(unsigned long) * GRAY_LEVELS);
    if(histogram == NULL) {
        allocFailed = true;
    }
    
    double* normalizedHistogram = malloc(sizeof(double) * GRAY_LEVELS);
    if(normalizedHistogram == NULL) {
        allocFailed = true;
    }
    
    double* cumulativeHistogram = malloc(sizeof(double) * GRAY_LEVELS);
    if(cumulativeHistogram == NULL) {
        allocFailed = true;
    }
    
    unsigned short* kShift = malloc(sizeof(unsigned short) * GRAY_LEVELS);
    if(kShift == NULL) {
        allocFailed = true;
    }
    
    if(allocFailed == false) {
        calculateHistogram(xSize, ySize, inputImage, histogram);
        normalizeHistogram(histogram, normalizedHistogram, xSize, ySize);
        cumulateHistogram(normalizedHistogram, cumulativeHistogram);
        
        //find the lowest and highest values
        unsigned short darkestPixel;
        for(darkestPixel = 0; darkestPixel < GRAY_LEVELS; darkestPixel++) {
            if(histogram[darkestPixel] != 0) {
                break;
            }
        }
        
        unsigned short brightestPixel;
        for(brightestPixel = GRAY_LEVELS-1; brightestPixel >= 0; brightestPixel--) {
            if(histogram[brightestPixel] != 0) {
                break;
            }
        }
        
        //Calculate k-shift
        double scalingFactor = (double)(GRAY_LEVELS - 1) / (double)(cumulativeHistogram[brightestPixel] - cumulativeHistogram[darkestPixel]);
        for(unsigned short i = 0; i < GRAY_LEVELS; i++) {
            double newValue = scalingFactor * (cumulativeHistogram[i] - cumulativeHistogram[darkestPixel]);
            if(newValue < 0) {
                newValue = 0;
            }
            kShift[i] = round(newValue);
        }
        
            printf("brightness, histogram, normalized, cumulative, new\n");
        for(unsigned short i = 0; i < GRAY_LEVELS; i++) {
            printf("       %3i, %9lu,      %1.3f,      %1.3f, %3i\n", i, histogram[i], normalizedHistogram[i], cumulativeHistogram[i], kShift[i]);
        }
        
        for(unsigned long i = 0; i < (unsigned long) xSize * (unsigned long) ySize; i++) {
            outputImage[i] = kShift[inputImage[i]];
        }
    }

    free(histogram);
    free(normalizedHistogram);
    free(cumulativeHistogram);
    
    return allocFailed;
}
