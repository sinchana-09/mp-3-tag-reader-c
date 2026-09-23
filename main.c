#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void display_help();
void view_tags(char filename[]);
void edit_tag(char edit_option[], char new_value[]);
int main()
{
    char option;
    printf("1. Help\n");
    printf("2. View\n");
    printf("3. Edit\n");
    printf("Enter an option :");
    scanf("%c" , &option);
    switch (option)
    {
        case '1':
        display_help();
        break;
        case '2':
            view_tags("backup.mp3.mpeg");
            break;
        case '3':
            char option[10];
            char value[100];
            printf("Enter tag modifier(-t for Title , -a for Artist , A for Album, -y for year, -m for content, -c for comment):");
            scanf("%s" , option);
            printf("Enter new value: ");
            scanf(" %[^\n]" , value);
            edit_tag(option ,value);
            break;
            default:
                printf("ERROR: Invalid option.\n");
                break;
    }
    return 0;
}
