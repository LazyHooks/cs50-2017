#define FAT_PARTITION 512     // as Fat Stores data in 512 byte partitions

#include <stdio.h>
#include <stdint.h>


int main(int argc, char* argv[])
{
    if(argc!=2)          //check for proper usage
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    char *inputfile = argv[1];
    FILE *input = fopen(inputfile,"r");  //source FILE in read mode
    
    if(input==NULL)
    {
        fprintf(stderr, "Could not open specified image %s.\n", inputfile);
        return 2;
    }
    
    uint8_t toBeWritten[FAT_PARTITION];    //buffer to read a FAT block of input bytes to later be written to FILE of size 8bits = 1 byte
    
    int FILEIndex=0; //to number the Files accordingly
    char FILEName[10];
    int newFILECreated=0;  
    FILE *currentOutput=NULL; //to output separate images
    
    while(fread(toBeWritten,FAT_PARTITION,1,input))  //loop to run over contents until it has no more partitions (reads one entire partition at a time)
    {
        //check if new FILE gets created or previous one continues
        newFILECreated=0;
        
        // to check first 4 bytes to match with jpeg syntax
        if(toBeWritten[0]==0xff && toBeWritten[1]==0xd8 && toBeWritten[2]==0xff && (toBeWritten[3]==0xe0 || toBeWritten[3]==0xe1 || toBeWritten[3]==0xe2 || toBeWritten[3]==0xe3 || toBeWritten[3]==0xe4 || toBeWritten[3]==0xe5 || toBeWritten[3]==0xe6 || toBeWritten[3]==0xe7 || toBeWritten[3]==0xe8 || toBeWritten[3]==0xe9 || toBeWritten[3]==0xea || toBeWritten[3]==0xeb || toBeWritten[3]==0xec || toBeWritten[3]==0xed || toBeWritten[3]==0xee || toBeWritten[3]==0xef))
        {
            //check if previous FILE is opened
            if(currentOutput != NULL)
               fclose(currentOutput); // close it as previous FILE has ended
               
            sprintf(FILEName,"%03d.jpg",FILEIndex); //creates a FILEname as Index.jpg (Index upto 3 digits)
            FILEIndex = FILEIndex+1;
            currentOutput = fopen(FILEName,"w"); //opens new FILE as per FILEName in write mode
            newFILECreated =1;
            fwrite(toBeWritten,FAT_PARTITION,1,currentOutput); //writes the block to the newly created FILE
        }
        else if(currentOutput!=NULL && newFILECreated==0)
        {
            fwrite(toBeWritten,FAT_PARTITION,1,currentOutput);
        }
    }
    
    if(currentOutput!=NULL)
       fclose(currentOutput); //closes the last FILE

    //success
    return 0;
}

