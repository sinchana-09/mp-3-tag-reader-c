#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void edit_tag(char edit_option[], char new_value[]) 
{
    // Convert user option to 4-byte Tag ID
    char target_tag[5] = {0};
    if (strcmp(edit_option, "-t") == 0) 
    strcpy(target_tag, "TIT2");
    if (strcmp(edit_option, "-a") == 0) 
    strcpy(target_tag, "TPE1");
    if (strcmp(edit_option, "-A") == 0) 
    strcpy(target_tag, "TALB");
    if (strcmp(edit_option, "-y") == 0) 
    strcpy(target_tag, "TYER");
    if (strcmp(edit_option, "-m") == 0) 
    strcpy(target_tag, "TCON");
    if (strcmp(edit_option, "-c") == 0) 
    strcpy(target_tag, "COMM");


    // Open source file in read mode and temp file in write mode
    FILE *fp_src = fopen("backup.mp3.mpeg", "rb");
    FILE *fp_dest = fopen("temp.mp3", "wb");

    if (fp_src == NULL || fp_dest == NULL)
    {
        printf("Error: File opening failed.\n");
        return;
    }

    // Copy first 10 bytes header directly
    char header[10];
    fread(header, 1, 10, fp_src);
    fwrite(header, 1, 10, fp_dest);

    char tag[5] = {0};

    // Read through frames 4 bytes at a time
    while (fread(tag, 1, 4, fp_src) == 4)
    {
        // Read 4 size bytes
        unsigned char sz[4];
        fread(sz, 1, 4, fp_src);
        int old_size = (sz[0] << 24) | (sz[1] << 16) | (sz[2] << 8) | sz[3];

        if (strcmp(tag, target_tag) == 0)
        {
            // MATCH FOUND: Write tag and calculate new size
            int new_size = strlen(new_value) + 1;
            unsigned char new_sz[4];
            new_sz[0] = (new_size >> 24) & 0xFF;
            new_sz[1] = (new_size >> 16) & 0xFF;
            new_sz[2] = (new_size >> 8) & 0xFF;
            new_sz[3] = new_size & 0xFF;

            fwrite(tag, 1, 4, fp_dest);
            fwrite(new_sz, 1, 4, fp_dest);

            // Read and write 3 bytes (flags + encoding)
            char extra[3];
            fread(extra, 1, 3, fp_src);
            fwrite(extra, 1, 3, fp_dest);

            // Write new text value
            fwrite(new_value, 1, strlen(new_value), fp_dest);

            // Skip old text in source file
            fseek(fp_src, old_size - 1, SEEK_CUR);

            // Copy remaining audio data till EOF
            char ch;
            while (fread(&ch, 1, 1, fp_src) == 1)
            {
                fwrite(&ch, 1, 1, fp_dest);
            }
            break;
        } 
        else
        {
            // NO MATCH: Copy tag, size, flags, and content byte-by-byte
            fwrite(tag, 1, 4, fp_dest);
            fwrite(sz, 1, 4, fp_dest);

            char extra[3];
            fread(extra, 1, 3, fp_src);
            fwrite(extra, 1, 3, fp_dest);

            // Safe loop to copy content of any size without buffer overflow
            for (int i = 0; i < old_size - 1; i++)
            {
                char ch;
                if (fread(&ch, 1, 1, fp_src) == 1)
                {
                    fwrite(&ch, 1, 1, fp_dest);
                }
            }
        }
    }

    fclose(fp_src);
    fclose(fp_dest);

    // Replace original file with modified temp file
    remove("backup.mp3.mpeg");
    rename("temp.mp3", "backup.mp3.mpeg");

    printf("Tag edited successfully!\n");
}