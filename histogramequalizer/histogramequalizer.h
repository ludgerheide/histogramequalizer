//
//  histogramequalizer.h
//  histogramequalizer
//
//  Created by Ludger Heide on 11.10.16.
//  Copyright © 2016 Ludger Heide. All rights reserved.
//

#ifndef histogramequalizer_h
#define histogramequalizer_h

//Equalize the histogram of a grayscale image
//Expects an array of unsigned chars of xSize*zSize
int equalizehistogram(unsigned int xSize, unsigned int ySize, unsigned char* inputImage, unsigned char* outputImage);

#endif /* histogramequalizer_h */
