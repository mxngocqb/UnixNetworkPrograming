#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<sys/dir.h>
#include<stdlib.h>

int main() {
    // Mai Xuân Ngọc - 20204769
    struct dirent **entries;
    int num_entries, i;

    // Open the current directory
    DIR *dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
    }

    // Scan the current directory for entries
    num_entries = scandir(".", &entries, NULL, alphasort);
    if (num_entries < 0) {
        perror("scandir");
    }

    // Open the output HTML file for writing
    FILE *outfile = fopen("output.html", "w");
    if (outfile == NULL) {
        perror("fopen");
    }

    // Write the beginning of the HTML file
    fprintf(outfile, "<html>\n<body>\n");

    // Write a hyperlink to each file in the current directory
    for (i = 0; i < num_entries; i++) {
        fprintf(outfile, "<a href=\"%s\">%s</a><br>\n", entries[i]->d_name, entries[i]->d_name);
        free(entries[i]);
    }

    // Write the end of the HTML file
    fprintf(outfile, "</body>\n</html>\n");
    free(entries);

    // Close the output file and the directory
    fclose(outfile);
    closedir(dir);

    return 0;
}
