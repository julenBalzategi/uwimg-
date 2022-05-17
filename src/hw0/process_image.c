#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0) {
        x = 0;
    } else if (x >= im.w) {
        x = im.w - 1;
    }
    if (y < 0) {
        y = 0;
    } else if (y >= im.h) {
        y = im.h - 1;
    }
    // channel clamping really shouldn't ever happen
    if (c < 0) {
        fprintf(stderr, "get_pixel clamping from negative on c=%d", c);
        c = 0;
    } else if (c >= im.c) {
        fprintf(stderr, "get_pixel clamping from positive on c=%d", c);
        c = im.c - 1;
    }
    return im.data[(im.w*im.h*c)+(im.w*y)+(x)];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if ((x >= 0) && (x < im.w) && (y >= 0) && (y < im.h) && (c >= 0) && c < im.c){
        im.data[(im.w*im.h*c)+(im.w*y)+(x)] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    for (int z = 0; z < im.c*(im.w*im.h) ; z = z + (im.w*im.h)){
        for (int j = 0; j < (im.w*im.h) ; j = j +(im.w)){
            for (int i = 0; i < im.w ; i++){
                copy.data[i+j+z] = im.data[i+j+z];
            }
        }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int i = 0; i < im.w ; i++){
        for (int j = 0; j < im.h ; j++){
            float weight = 0;
            weight += 0.299 * get_pixel(im, i, j, 0);
            weight += 0.587 * get_pixel(im, i, j, 1);
            weight += 0.114 * get_pixel(im, i, j, 2);
            set_pixel(gray, i, j, 0, weight);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int i = 0; i < im.w; i++) {
        for (int j = 0; j < im.h; j++) {
            float newvalue = get_pixel(im, i, j, c) + v;
            set_pixel(im, i, j, c, newvalue);
        }
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int c = 0; c < im.c; c++) {
        for (int y = 0; y < im.h; y++) {
            for (int x = 0; x < im.w; x++) {
                float value = get_pixel(im, x, y, c);
                if (value < 0) {
                    value = 0;
                } else if (value > 1) {
                    value = 1;
                }
                set_pixel(im, x, y, c, value);
            }
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int y = 0; y < im.h; y++) {
        for (int x = 0; x < im.w; x++) {
            // get the rgb values to start with
            float r = get_pixel(im, x, y, 0);
            float g = get_pixel(im, x, y, 1);
            float b = get_pixel(im, x, y, 2);
            // calculate Value
            float v = three_way_max(r, g, b);
            // calculate Saturation
            float m = three_way_min(r, g, b);
            float c = v - m;
            float s;
            if (r==0 && g==0 && b==0) {
                s = 0;
            } else {
                s = c / v;
            }
            // calculate Hue
            float hp;
            if (c==0) {
                hp = 0;
            } else if (v==r) {
                hp = (g - b) / c;
            } else if (v==g) {
                hp = (b - r) / c + 2;
            } else { // if v==b
                hp = (r - g) / c + 4;
            }
            float h;
            if (hp<0) {
                h = hp / 6 + 1;
            } else {
                h = hp / 6;
            }
            // Apply the values
            set_pixel(im, x, y, 0, h);
            set_pixel(im, x, y, 1, s);
            set_pixel(im, x, y, 2, v);
        }
    }
}

float hsv_to_rgb_func(float h, int n, float v, float s)
{
    //https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
    float k = fmodf(n + (h / 60), 6);
    return v - v * s * fmaxf(0, three_way_min(k, 4 - k, 1));
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in

    for (int j = 0; j < im.h; j++) {
        for (int i = 0; i < im.w; i++) {
            // get the rgb values to start with
            float h = get_pixel(im, i, j, 0);
            float s = get_pixel(im, i, j, 1);
            float v = get_pixel(im, i, j, 2);
            h = h * 360;
            float r = hsv_to_rgb_func(h, 5, v, s);
            float g = hsv_to_rgb_func(h, 3, v, s);
            float b = hsv_to_rgb_func(h, 1, v, s);

            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}
