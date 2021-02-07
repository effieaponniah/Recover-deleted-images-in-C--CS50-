/*
  This program was written by Effiea on October 12th 2020
  Last modified December 29th 2020
  This program retrieves deleted JPEG files (photos) from
  the memory card.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // Open deleted raw memory card file
    FILE *file = fopen(argv[1], "r");

    FILE *file_pointer;
    uint8_t data_pointer[512];
    size_t number_of_elements = 1;
    int filenumber = 0;
    bool found_jpeg = false;
    bool first_jpeg = false;
    char filename[30];

    // Validate if there are no recovery file name provided to recover images
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Continue with program is there is a valid input
    else
    {
        // Repeat until number of elements is zero
        while (number_of_elements != 0)
        {
            // Read 512 bytes at a time into data_pointer variable
            number_of_elements = fread(data_pointer, sizeof(uint8_t), 512, file);

            if (number_of_elements != 0)
            {
                // If statement for the start of a new JPEG
                if (data_pointer[0] == 0xff && data_pointer[1] == 0xd8 && data_pointer[2] == 0xff && (data_pointer[3] & 0xf0) == 0xe0)
                {
                    // Found first JPEG file
                    if (!first_jpeg)
                    {
                        first_jpeg = true;
                    }

                    // Create new recovery file names as per specification
                    sprintf(filename, "%03i.jpg", filenumber);
                    // Open a new file to write the deleted content
                    file_pointer = fopen(filename, "w");
                    // Write the deleted content into a new file
                    fwrite(data_pointer, sizeof(uint8_t), number_of_elements, file_pointer);

                    // Close the file pointer everytime after writing to the new file
                    if (!first_jpeg)
                    {
                        fclose(file_pointer);
                    }

                    // increase file number to create new recovery files
                    filenumber++;
                    found_jpeg = true;

                }
                else if (found_jpeg)
                {
                    // Recover the very first JPEG file from the deleted memory card
                    fwrite(data_pointer, sizeof(uint8_t), number_of_elements, file_pointer);
                }
            }

            else
            {
                fclose(file);
                return 0;
            }
        }

        fclose(file);
        fclose(file_pointer);
        return 0;
    }
}
