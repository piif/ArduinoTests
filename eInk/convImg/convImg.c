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

void main(int argc, char **argv) {
    char *name="TO_REPLACE";
    if (argc == 2) {
        name = argv[1];
    }
    printf(
        "#include \"image4b.h\"\n"
        "\n"
        "unsigned char %s_image_data[] = {\n",
        name
    );
    char l[100];
    int w = 0, h = 0;
    while(fgets(l, 99, stdin)) {
        char prev = l[0];
        char count = 0;
        int i;
        printf("    ");
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
    printf(
        "};\n"
        "\n"
        "Image4b %s_image = {\n"
        "    width: %d,\n"
        "    height: %d,\n"
        "    data_len: %d,\n"
        "    compressed_data: %s_image_data\n"
        "};\n",
        name, w, h, total_count, name
    );
}
