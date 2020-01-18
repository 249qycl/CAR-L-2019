#ifndef __TOOLS_H__
#define __TOOLS_H__

int16 First_LowPassFilter(int16 input);
int16 First_HighPassFilter(int16 input);
int16 RecursiveMeanFilter(int16 input);
float Least_square_method1(float input,uint8 num,uint8 interval);
float Least_square_method2(float input,uint8 interval);
#endif