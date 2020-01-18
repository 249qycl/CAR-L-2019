#include "common.h"
#include "include.h"


uint16 Red_distance()
{
    uint16 distance=0;
    float temp;
    uint32 var=0;
    for(uint8 i=0;i<10;i++)
    {
        var += adc_once(ADC0_SE10, ADC_10bit);
        DELAY_US(10);
    }
    distance=var/10;
    temp=(float)(distance-200)/750;
    distance=(uint16)(190*temp*temp*temp*temp-490*temp*temp*temp+460*temp*temp-200*temp+48);
    return distance;
}