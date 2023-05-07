#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buffer[1024];
    char command[] = "ifconfig";
    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        perror("popen");
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        printf("%s\n", buffer);
    }
    pclose(fp);

    strcpy(command, "ls -a -l");
    fp = popen(command, "r");
    if (fp == NULL)
    {
        printf("Failed to execute command\n");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        printf("%s\n", buffer);
    }
    pclose(fp);
    return 0;
}
