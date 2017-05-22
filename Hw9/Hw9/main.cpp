//
//  main.cpp
//  Hw9
//
//  Created by Antonio Tsai on 22/05/2017.
//  Copyright © 2017 TTU. All rights reserved.
//
//  tess.cpp:
//  Recognize text on an image using Tesseract API and print it to the screen
//  Usage: ./tess image.png


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/strngs.h>
#include <iostream>

using namespace tesseract;

int main(int argc, const char * argv[]) {
    char* image_path = "";
    char *outText;
    
    TessBaseAPI *api = new TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "eng")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }
    
    // Open input image with leptonica library
    Pix *image = pixRead("/usr/src/tesseract/testing/phototest.tif");
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();
    printf("OCR output:\n%s", outText);
    
    // Destroy used object and release memory
    api->End();
    delete [] outText;
    pixDestroy(&image);
    
    return 0;
}

