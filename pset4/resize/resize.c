/**
 * Resizes a BMP by a multiplier input by the user in range of 1 to 100
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc !=4)
    {
        fprintf(stderr, "Usage: ./resize n(multiplier) infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    
    // remember multiplying factor
    int multiplier= atoi(argv[1]);
    
    //check for resize multiplier in range
    if(multiplier>100 && multiplier <1)
    {
        printf(" Enter a resize multiplier between 1 and 100");
        return 1;
    }

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //to determine new parameters for output file so as to define it
    BITMAPINFOHEADER bi_final;                                    // bitmap info header variable for output file
    bi_final=bi;                                                  // copies everything so i can change only the ones i want and use it
    bi_final.biHeight = multiplier*bi.biHeight;
    bi_final.biWidth=multiplier*bi.biWidth;
    
     // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int pad_final =(4-(bi_final.biWidth*sizeof(RGBTRIPLE))%4)%4;
    
    // new bitmap size calculation
    bi_final.biSizeImage = (bi_final.biWidth * sizeof(RGBTRIPLE) + pad_final) * abs(bi_final.biHeight);
    BITMAPFILEHEADER bf_final;                                               // new bitmap file header variable / object 
    bf_final=bf;                                                             // sets it to original values so only necessary fields can be changed
    bf_final.bfSize = bf.bfSize - bi.biSizeImage + bi_final.biSizeImage;     // new size = original + change in image size (as other fields have same size)
    
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_final, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_final, sizeof(BITMAPINFOHEADER), 1, outptr);

   

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // loop as many times as multiplier printing the same line for resizing height wise
        for(int i2=0; i2<multiplier;i2++)
        {
            
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile , but as many times as multiplier to resize breadth wise
            for(int j2=0;j2<multiplier;j2++)
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            
        }
        
        // inserts new padding as per new padding amount calculated
        for (int p = 0; p < pad_final; p++)
        fputc(0x00, outptr);
        
        //returns cursor back to the front of the line to be read(repeatedly) as per original constraints to read original file multiplier no of times
        if (i2 < multiplier-1)  // multiplier-1 as cursor need not be returned to original read position after reading the last line
        fseek(inptr, -sizeof(RGBTRIPLE)*bi.biWidth, SEEK_CUR);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
