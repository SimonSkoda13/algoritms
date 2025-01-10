#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 1000

typedef struct
{
    int width;
    int height;
    int **pixels;
} Image;

typedef struct point
{
    int x;
    int y;
} Point;

typedef struct square
{
    Point start;
    Point end;
} Square;

void print_help();
int load_image(const char *filename, Image *image);
void free_image(Image *image);
int is_valid_image(const Image *image);
void test(const char *filename);
void find_hline(const Image *image);
void find_vline(const Image *image);
void find_square(const Image *image);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Invalid\n");
        return 1;
    }

    if (strcmp(argv[1], "--help") == 0)
    {
        print_help();
        return 0;
    }

    // overenie argumentov
    if (argc != 3)
    {
        fprintf(stderr, "Invalid\n");
        return 1;
    }

    const char *command = argv[1];
    const char *filename = argv[2];

    // Nacitanie obrazka
    Image image;
    if (!load_image(filename, &image))
    {
        fprintf(stderr, "Invalid\n");
        return 1;
    }

    // Spracovanie funkcii
    if (strcmp(command, "test") == 0)
    {
        test(filename);
    }
    else if (strcmp(command, "hline") == 0)
    {
        find_hline(&image);
    }
    else if (strcmp(command, "vline") == 0)
    {
        find_vline(&image);
    }
    else if (strcmp(command, "square") == 0)
    {
        find_square(&image);
    }
    else
    {
        fprintf(stderr, "Invalid\n");
        free_image(&image);
        return 1;
    }

    free_image(&image);
    return 0;
}

void print_help()
{
    printf("Pouzitie\n");
    printf("  ./figsearch --help\n");
    printf("  ./figsearch test SUBOR\n");
    printf("  ./figsearch hline SUBOR\n");
    printf("  ./figsearch vline SUBOR\n");
    printf("  ./figsearch square SUBOR\n");
    printf("\nPrikazy\n");
    printf("  --help        Zobrazi tuto spravu o pouziti programu\n");
    printf("  test          Overi platnost suboru bitmapy\n");
    printf("  hline         Najde prvu najdlhsiu horizontalnu ciaru\n");
    printf("  vline         Najde prvu najdlhsiu vertikalnu ciaru\n");
    printf("  square        Najde prvy najvacsi stvorec\n");
}

// Nacita obrazok z bitmapy
int load_image(const char *filename, Image *image)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        return 0;
    }

    // velkost obrazka
    if (fscanf(file, "%d %d", &image->height, &image->width) != 2)
    {
        fclose(file);
        return 0;
    }

    // nespravna velkost
    if (image->height <= 0 || image->width <= 0 || image->height > MAX_SIZE || image->width > MAX_SIZE)
    {
        fclose(file);
        return 0;
    }

    // alokacia pamate pre pixely
    image->pixels = malloc(image->height * sizeof(int *));

    // nepodarilo sa alokovat
    if (!image->pixels)
    {
        fclose(file);
        return 0;
    }

    for (int i = 0; i < image->height; i++)
    {
        image->pixels[i] = malloc(image->width * sizeof(int));
        if (!image->pixels[i])
        {
            fclose(file);
            return 0;
        }
    }

    // Citanie dat
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            if (fscanf(file, "%d", &image->pixels[i][j]) != 1 || (image->pixels[i][j] != 0 && image->pixels[i][j] != 1))
            {
                fclose(file);
                return 0; // nevalidny pixel
            }
        }
    }
    int extra;
    if (fscanf(file, "%d", &extra) == 1)
    {
        fclose(file);
        return 0; // nevalidny obrazok, ak su dalsie pixely
    }

    fclose(file);
    return 1;
}

// Free pamat
void free_image(Image *image)
{
    for (int i = 0; i < image->height; i++)
    {
        free(image->pixels[i]);
    }
    free(image->pixels);
}

// Testovanie validity obrazka
int is_valid_image(const Image *image)
{
    if (image->height <= 0 || image->width <= 0)
    {
        return 0;
    }
    for (int i = 0; i < image->height; i++)
    {
        for (int j = 0; j < image->width; j++)
        {
            if (image->pixels[i][j] != 0 && image->pixels[i][j] != 1)
            {
                return 0;
            }
        }
    }
    return 1;
}

// Vypis testu file
void test(const char *filename)
{
    Image image;
    if (!load_image(filename, &image))
    {
        fprintf(stderr, "Invalid\n");
        return;
    }

    if (is_valid_image(&image))
    {
        printf("Valid\n");
    }
    else
    {
        fprintf(stderr, "Invalid\n");
    }

    free_image(&image);
}

void find_hline(const Image *image)
{
    int maxLength = 0;
    int finalRow = -1, finalStartColumn = -1, finalEndColumn = -1;
    for (int i = 0; i < image->height; i++)
    {
        int length = 0, start = -1;
        for (int j = 0; j < image->width; j++)
        {
            if (image->pixels[i][j] == 1)
            {
                if (length == 0)
                {
                    start = j;
                }
                length++;
            }
            else
            {
                if (length > maxLength)
                {
                    maxLength = length;
                    finalRow = i;
                    finalStartColumn = start;
                    finalEndColumn = j - 1;
                }
                length = 0;
            }
        }
        if (length > maxLength)
        {
            maxLength = length;
            finalRow = i;
            finalStartColumn = start;
            finalEndColumn = image->width - 1;
        }
    }
    if (maxLength == 0)
    {
        printf("Not found");
    }
    else
    {
        printf("%d %d %d %d\n", finalRow, finalStartColumn, finalRow, finalEndColumn);
    }
}

void find_vline(const Image *image)
{
    int maxLength = 0;
    int finalCol = -1, finalStartRow = -1, finalEndRow = -1;
    for (int j = 0; j < image->width; j++)
    {
        int length = 0, start = -1;
        for (int i = 0; i < image->height; i++)
        {
            if (image->pixels[i][j] == 1)
            {
                if (length == 0)
                {
                    start = i;
                }
                length++;
            }
            else
            {
                if (length == maxLength && start < finalStartRow)
                {
                    finalStartRow = start;
                    finalCol = j;
                    finalEndRow = i - 1;
                }
                else if (length > maxLength)
                {
                    maxLength = length;
                    finalCol = j;
                    finalStartRow = start;
                    finalEndRow = i - 1;
                }
                length = 0;
            }
        }
        if (length == maxLength && start < finalStartRow)
        {
            finalStartRow = start;
            finalCol = j;
            finalEndRow = image->height - 1;
        }
        else if (length > maxLength)
        {
            maxLength = length;
            finalCol = j;
            finalStartRow = start;
            finalEndRow = image->height - 1;
        }
    }
    if (maxLength == 0)
    {
        printf("Not found");
    }
    else
    {
        printf("%d %d %d %d\n", finalStartRow, finalCol, finalEndRow, finalCol);
    }
}

void find_square(const Image *image)
{
    Point start = {-1, -1};
    Point end = {-1, -1};
    int size = 0;
    for (int i = 0; i < image->height; i++)
    {

        for (int j = 0; j < image->width; j++)
        {
            if (start.x == -1)
            {
                size = 0;
            }
            else
            {
                size = end.x - start.x + 1;
            }
            while (size + j <= image->width - 1 && size + i <= image->height - 1)
            {
                int found = 0;

                for (int x = i; x <= i + size; x++)
                {

                    if (image->pixels[x][j] != 1 || image->pixels[x][j + size] != 1)
                    {
                        break;
                    }
                    else if (x == i + size)
                    {
                        found = 1;
                    }
                    // printf("X- i:%d j:%d x:%d found:%d size:%d\n", i, j, x, found, size);
                }

                for (int y = j; y <= j + size; y++)
                {
                    // printf("Yb- i:%d j:%d x:%d found:%d size:%d\n", i, j, y, found, size);

                    if (image->pixels[i][y] != 1 || image->pixels[i + size][y] != 1)
                    {
                        break;
                    }
                    else if (y == j + size)
                    {
                        found = 2;
                    }
                    // printf("Ya- i:%d j:%d x:%d found:%d size:%d\n", i, j, y, found, size);
                }
                if (found == 2)
                {
                    start.x = i;
                    start.y = j;
                    end.x = i + size;
                    end.y = j + size;
                }

                size++;
            }
        }
    }
    if (start.x == -1)
    {
        printf("Not found");
    }
    else
    {
        printf("%d %d %d %d\n", start.x, start.y, end.x, end.y);
    }
}