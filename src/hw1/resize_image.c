#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    image ret_img = make_image(w,h,im.c);
    float old_x_top, old_y_top, old_x_bottom, old_y_bottom, new_x_bottom, new_x_top, new_y_bottom, new_y_top, ax, ay, bx, by, old_x, old_y;


    old_x_top = -0.5;
    old_x_bottom = im.w - 0.5;

    old_y_top = -0.5;
    old_y_bottom = im.h - 0.5;

    new_x_top = -0.5;
    new_x_bottom = w - 0.5;

    new_y_top = -0.5;
    new_y_bottom = h - 0.5;

    ax = (old_x_bottom - old_x_top) / (new_x_bottom - new_x_top);
    bx = old_x_top - (ax * new_x_top);

    ay = (old_y_bottom - old_y_top) / (new_y_bottom - new_y_top);
    by = old_y_top - (ay * new_y_top);


    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            for(int z = 0; z < im.c; z++){
                old_x = ax * i + bx; //we apply to get what should be old values to interpolate and obtain the new ones
                old_y = ay * j + by;
                set_pixel(ret_img, i, j, z, nn_interpolate(im, old_x, old_y, z));

            }
        }
    }

    return ret_img;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    float right, left, top, bottom;

    top = floorf(y);
    bottom = ceilf(y);
    left = floorf(x);
    right = ceilf(x);


    float q1 = (right - x) * get_pixel(im, left, top, c) + (x - left) * get_pixel(im, right, top, c);
    float q2 = (right - x) * get_pixel(im, left, bottom, c) + (x - left) * get_pixel(im, right, bottom, c);


    return q1 * (bottom - y) + q2 * (y - top);

}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    image ret_img = make_image(w,h,im.c);
    float old_x_top, old_y_top, old_x_bottom, old_y_bottom, new_x_bottom, new_x_top, new_y_bottom, new_y_top, ax, ay, bx, by, old_x, old_y;


    old_x_top = -0.5;
    old_x_bottom = im.w - 0.5;

    old_y_top = -0.5;
    old_y_bottom = im.h - 0.5;

    new_x_top = -0.5;
    new_x_bottom = w - 0.5;

    new_y_top = -0.5;
    new_y_bottom = h - 0.5;


    //This is just the result of the equations. We want to be able to get old image values so we can interpolate to obtain the new values
    //general equation idea: new_val * a + b = old_val.
    //new_bottom_x * a + b = old_bottom_x
    //new_bottom_y * a + b = old_bottom_y
    //new_top_x * a + b = old_top_x
    //new_top_y * a + b = old_top_y

    ax = (old_x_bottom - old_x_top) / (new_x_bottom - new_x_top);
    bx = old_x_top - (ax * new_x_top);

    ay = (old_y_bottom - old_y_top) / (new_y_bottom - new_y_top);
    by = old_y_top - (ay * new_y_top);


    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            for(int z = 0; z < im.c; z++){
                old_x = ax * i + bx; //we apply to get what should be old values to interpolate and obtain the new ones
                old_y = ay * j + by;
                set_pixel(ret_img, i, j, z, bilinear_interpolate(im, old_x, old_y, z));

            }
        }
    }

    return ret_img;
}
