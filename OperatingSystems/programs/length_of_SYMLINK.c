#include <unistd.h>
#include <stdio.h>

// Before start create directory 'try' in this directory

int main()
{
    FILE* file;
    file = fopen("try/l0", "w");
    char str[100];
    char name[100];
    fclose (file);
    int error;
    int i = 0;
    int pr = 0;
    while (pr < 1000) {
        sprintf(str, "l%d", i);
        sprintf(name, "try/l%d", ++i);
        error = symlink(str, name);
        if (error < 0) {
            printf("You should delete directory and create empty dir 'try'\n");
            break;
        }
        file = fopen(name, "w+");
        if (file == NULL) {
            printf("Result: %d\n", --i); // On my PK it was 41
            break;
        }
        fclose (file);
        pr++;
    }
    return 0;

}
