#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_pixel(unsigned char *img, int width, int height, int channels, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    int index = (y * width + x) * channels;
    img[index + 0] = r;
    img[index + 1] = g;
    img[index + 2] = b;
}

void generate_grid_overlay(const char *image_path, int columns, int rows) {
    int width, height, channels;
    unsigned char *img = stbi_load(image_path, &width, &height, &channels, 3); 
    if (!img) {
        fprintf(stderr, "Failed to load image %s\n", image_path);
        return;
    }
    
    printf("Image loaded: %dx%d, channels: %d\n", width, height, channels);
    printf("Creating grid: %d columns x %d rows\n", columns, rows);
    
    channels = 3; 
    int line_thickness = 2;
    
    for (int c = 1; c < columns; c++) {
        int x = (c * width) / columns;
        printf("Drawing vertical line at x=%d\n", x);
        for (int y = 0; y < height; y++) {
            for (int t = 0; t < line_thickness; t++) {
                set_pixel(img, width, height, channels, x + t, y, 255, 255, 255);
            }
        }
    }
    
    for (int r = 1; r < rows; r++) {
        int y = (r * height) / rows;
        printf("Drawing horizontal line at y=%d\n", y);
        for (int x = 0; x < width; x++) {
            for (int t = 0; t < line_thickness; t++) {
                set_pixel(img, width, height, channels, x, y + t, 255, 255, 255); 
            }
        }
    }
    
    stbi_write_png("grid_overlay.png", width, height, channels, img, width * channels);
    stbi_image_free(img);
    printf("Saved grid overlay to grid_overlay.png\n");
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s --image <path> --canvas <columns>x<rows>\n", argv[0]);
        return 1;
    }
    
    const char *image_path = NULL;
    int columns = 0;
    int rows = 0;
    
    for (int i = 1; i < argc - 1; i++) {
        if (strcmp(argv[i], "--image") == 0) {
            image_path = argv[i + 1];
        } else if (strcmp(argv[i], "--canvas") == 0) {
            char *canvas_str = argv[i + 1];
            char *x_pos = strchr(canvas_str, 'x');
            if (x_pos == NULL) {
                fprintf(stderr, "Invalid canvas format. Use: COLUMNSxROWS (e.g., 3x4)\n");
                return 1;
            }
            
            *x_pos = '\0';  
            columns = atoi(canvas_str);
            rows = atoi(x_pos + 1);
            *x_pos = 'x';  
            
            if (columns <= 0 || rows <= 0) {
                fprintf(stderr, "Invalid canvas size: %dx%d\n", columns, rows);
                return 1;
            }
        }
    }
    
    if (image_path == NULL) {
        fprintf(stderr, "No image path specified\n");
        return 1;
    }
    
    printf("Processing with %d columns x %d rows\n", columns, rows);
    generate_grid_overlay(image_path, columns, rows);
    return 0;
}
