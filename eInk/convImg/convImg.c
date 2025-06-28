#include <stdio.h>

int total_count = 0;

void output(char chr, char count) {
    while (count > 15) {
        printf("0x%c%x, ", chr, 15);
        count -=16;
        total_count++;
    }
    printf("0x%c%x, ", chr, count);
    total_count++;
}

void main() {
    printf("Image4b TO_REPLACE = {\n");
    printf("    compressed_data = {\n");
    char l[100];
    int w = 0, h = 0;
    while(fgets(l, 99, stdin)) {
        char prev = l[0];
        char count = 0;
        int i;
        printf("        ");
        for (i = 1; l[i] != '\0' && l[i] != ' '; i++) {
            if (l[i]==prev) {
                count++;
            } else {
                output(prev, count);
                prev = l[i];
                count = 0;
            }
        }
        if (w == 0) {
            w = i;
        }
        output(prev, count);
        putchar('\n');
        h++;
    }
    printf("    },\n");
    printf("    width: %d,\n", w);
    printf("    height: %d,\n", h);
    printf("    data_len: %d\n", total_count);
    printf("};\n");
}
