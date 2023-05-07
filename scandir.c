#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
char *html = NULL;

void Append(char **phtml, const char *name)
{
    int oldLen = *phtml == NULL ? 0 : strlen(*phtml);
    (*phtml) = (char *)realloc(*phtml, oldLen + strlen(name) + 1); // caasp phat them
    memset(*phtml + oldLen, 0, strlen(name) + 1);                  // dien 0 vao cap phat them
    sprintf(*phtml + oldLen, "%s", name);                          // noi xau
}

int main()
{
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, NULL, NULL);
    if (n == -1)
    {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

     if (n == -1) {
        printf("Error\n");
    }else
    {
        Append(&html,"<html>\n");
        while (n--) {
            Append(&html,"<a href = \"");
            Append(&html, namelist[n]->d_name);
            if (namelist[n]->d_type == DT_DIR)
            {
                Append(&html,"\"><b>");
                Append(&html, namelist[n]->d_name);
                Append(&html,"</b></a><br>\n");
            }else
            {
                Append(&html,"\"><i>");
                Append(&html, namelist[n]->d_name);
                Append(&html,"</i></a><br>\n");
            }
            free(namelist[n]);
        }
        Append(&html,"</html>\n");
        free(namelist);
        namelist = NULL;
        FILE* f = fopen("scandir.html", "wt");
        fputs(html, f);
        fclose(f);
        free(html);
        html = NULL;
    }
}