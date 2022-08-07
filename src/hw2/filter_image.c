#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float new_value;
    float channel_size = im.w*im.h;
    for(int z = 0; z < im.c; z++){//normalize per channel
        float sum = 0;
        for (int i = z * channel_size; i - (z * channel_size) < im.w * im.h; i++) {
            sum += im.data[i];
        }
        for (int i = z * channel_size; i - (z * channel_size) < im.w * im.h; i++) {
            im.data[i] /= sum;
        }

    }

}

image make_box_filter(int w)
{
    // TODO
    image ret = make_image(w,w,1);
    float val = 1.0/(w*w);
    for(int i = 0; i < w; i++){
        for(int j = 0; j < w; j++){
            set_pixel(ret, i, j, 0, val);
        }
    }
    

    return ret;
}


double apply_filter(image im, int x, int y, int c, image filter, int kc){


    int offset_x = filter.w/2;
    int offset_y = filter.h/2;
    double ret = 0;
    
    for(int j = 0; j < filter.h; j++){
        for(int i = 0; i < filter.w; i++){
            float pix1 = get_pixel(im, x+i-offset_x, y+j-offset_y, c);
            float pix2 = get_pixel(filter, i, j, kc);
            ret += pix1 * pix2;
        }
    }
   
    return ret;    
                
}


image convolve_image(image im, image filter, int preserve)
{
    
    // TODO
    image im_ret = make_image(im.w, im.h, im.c);
    for(int z = 0; z < im.c; z++){    
        for(int j = 0; j < im.h; j++){
            for(int i = 0; i < im.w; i++){
                if(im.c == filter.c){
                    set_pixel(im_ret, i,j,z, apply_filter(im, i, j, z, filter, z));
                }else{
                    set_pixel(im_ret, i,j,z, apply_filter(im, i, j, z, filter, 0));
                } 
            }
        }
    }

    if (preserve) return im_ret;

    image im_ret_mono = make_image(im.w,im.h,1);
    for(int j = 0; j < im.h; j++){
        for(int i = 0; i < im.w; i++){
            float new_value = 0;
            for(int z = 0; z < im.c; z++){
                new_value += get_pixel(im_ret, i, j, z);
            }
            set_pixel(im_ret_mono, i, j, 0, new_value);
        }
    }    

    return im_ret_mono;
}

image make_highpass_filter()
{
    // TODO
    image ret = make_image(3,3,1);
    for(int j = 0; j < 3; j++){
        for(int i = 0; i < 3; i++){
            if(i%2 != 0 || j%2 != 0) set_pixel(ret, i, j, 0, -1);
            else set_pixel(ret, i, j, 0, 0);
        }
    set_pixel(ret, 1, 1, 0, 4);
    }
    return ret;
}

image make_sharpen_filter()
{
    // TODO
    image ret = make_highpass_filter();
    set_pixel(ret, 1, 1, 0, 5);
    return ret;
}

image make_emboss_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    ret.data[0] = -2;
    ret.data[1] = -1;
    ret.data[2] = 0;
    ret.data[3] = -1;
    ret.data[4] = 1;
    ret.data[5] = 1;
    ret.data[6] = 0;
    ret.data[7] = 1;
    ret.data[8] = 2;

    return ret;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    int sz = 6 * sigma;
    if (sz % 2 == 0) sz++;
    int half_sz = sz / 2;

    image ret = make_image(sz, sz, 1);

    float coeff = 1.0 / (TWOPI * sigma * sigma);
    float new_value = 0.;

    for(int i = -half_sz; i <= half_sz; i++){
        for(int j = -half_sz; j <= half_sz; j++){

            float exp = -(i * i + j * j) / (2.0 * sigma * sigma);
            new_value = coeff * powf(M_E, exp);
            //printf("%f", new_value);
            set_pixel(ret, i+half_sz, j+half_sz, 0, new_value);
        }    
    }
    //for (int i = 0; i < sigma*2+1; ++i)
      //  for (int j = 0; j < sigma*2+1; ++j)
        //    set_pixel(ret, i, j, 1, get_pixel(ret, i, j, 1)/sum);
    l1_normalize(ret);
    return ret;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) ret.data[i] = a.data[i] + b.data[i];
    return ret;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image ret = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.w * a.h * a.c; i++) ret.data[i] = a.data[i] - b.data[i];
    return ret;
}

image make_gx_filter()
{
    // TODO
    image ret = make_image(3, 3, 1);
    ret.data[0] = -1;
    ret.data[1] = 0;
    ret.data[2] = 1;
    ret.data[3] = -2;
    ret.data[4] = 0;
    ret.data[5] = 2;
    ret.data[6] = -1;
    ret.data[7] = 0;
    ret.data[8] = 1;

    return ret;
}

image make_gy_filter()
{
        // TODO
    image ret = make_image(3, 3, 1);
    ret.data[0] = -1;
    ret.data[1] = -2;
    ret.data[2] = -1;
    ret.data[3] = 0;
    ret.data[4] = 0;
    ret.data[5] = 0;
    ret.data[6] = 1;
    ret.data[7] = 2;
    ret.data[8] = 1;

    return ret;
}

void feature_normalize(image im)
{
    // TODO
    float min = 0;
    float max = 0;
    for(int i = 0; i < im.w*im.h*im.c; i++){
        min = MIN(min,im.data[i]);
        max = MAX(max,im.data[i]);
    }

    if(max-min == 0){
        for(int i = 0; i < im.w*im.h*im.c; i++){
                im.data[i] = 0;  
            }
    }else{
        for(int i = 0; i < im.w*im.h*im.c; i++){
            im.data[i] = (im.data[i] - min) / (max-min);  
        }
    }
    


}

image *sobel_image(image im)
{
    // TODO
    image gx = convolve_image(im, make_gx_filter(), 0);
    image gy = convolve_image(im, make_gy_filter(), 0);

    
    image gradient_img = make_image(im.w, im.h, 1);
    image grad_dir_img = make_image(im.w, im.h, 1);
    for (int i = 0; i < im.w * im.h; i++) {
      
        // gradient
        float gradient = sqrtf(gx.data[i] * gx.data[i] + gy.data[i] * gy.data[i]);
        gradient_img.data[i] = gradient;

        // gradient direction
        float grad_dir = atan2f(gy.data[i], gx.data[i]);
        grad_dir_img.data[i] = grad_dir;
    }

    image* ret =  calloc(2, sizeof(image));

    ret[0].w = gradient_img.w;
    ret[0].h = gradient_img.h;
    ret[0].c = gradient_img.c;
    ret[0].data = gradient_img.data;

    ret[1].w = grad_dir_img.w;
    ret[1].h = grad_dir_img.h;
    ret[1].c = grad_dir_img.c;  
    ret[1].data = grad_dir_img.data;


    return ret;
}

image colorize_sobel(image im)
{
    // TODO
    image gauss_filter = make_gaussian_filter(2);
    image smoothed = convolve_image(im, gauss_filter, 1);
    image* sobel = sobel_image(smoothed);

    feature_normalize(sobel[0]);
    feature_normalize(sobel[1]);

    image ret = make_image(im.w, im.h, im.c);
    for (int x = 0; x < im.w; x++) {
        for (int y = 0; y < im.h; y++) {
            // get gradient, dir
            double grad = get_pixel(sobel[0], x, y, 0);
            double dir = get_pixel(sobel[1], x, y, 0);

            set_pixel(ret, x, y, 0, dir);
            set_pixel(ret, x, y, 1, grad);
            set_pixel(ret, x, y, 2, grad);
        }
    }
    hsv_to_rgb(ret);
    return ret;

    return make_image(1,1,1);
}
