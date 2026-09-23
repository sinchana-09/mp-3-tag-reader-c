#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void view_tags(char filename[])
{
    FILE *fp = fopen("backup.mp3.mpeg" , "rb");
    if (fp== NULL)
    {
        printf("Error: Could not open file %s\n", filename);
        return ;
    }
    //Validate ID3 header (3bytes)
    char header[4]={0};
    fread(header, 1,3,fp);
    if(strcmp(header, "ID3") !=0)
    {
        printf("Error : Not a valid ID3v2 MP3 file.\n");
        fclose(fp);
        return;
    }
    printf("\n----------VIEW SELECTED----------\n\n");
    
    //Skip 7bytes(remaining part of 10-byte header)
    fseek(fp,7,SEEK_CUR);

    //Loop to read 6 main tags(TIT2 , TPE1, TALB, TYER, TCON,COMM)
        int i;
        for(i=1 ; i<=6; i++)
        {
            char tag[5]={0};
            if(fread(tag,1,4,fp) !=4)
            break;
        //Read 4-byte size stored in Big Endian
            unsigned char sz[4];
            fread(sz , 1,4,fp);
            int size =(sz[0]<<24|sz[1]<<16|sz[2]<<8)|sz[3];//Converting big endian to little endian
            //Skip 3 bytes(2bytes flags +1 byte encoding char)
            fseek(fp,3, SEEK_CUR);
            //Read the actual string
            char content[200]={0};
            fread(content , 1,size-1 , fp);
        //validation check before printing
        if(strcmp(tag, "TIT2")==0 || strcmp(tag, "TPE1")==0 || strcmp(tag, "TALB")==0|| strcmp(tag, "TYER")==0|| strcmp(tag, "TCON")==0 || strcmp(tag, "COMM")==0)
        {
            printf("%d.%5s --->%s\n" , i,tag, content);
        }
    }
        printf("\n--------------------\n");
        fclose(fp);
    }