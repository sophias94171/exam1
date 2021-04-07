#include "mbed.h"
#include "uLCD_4DGL.h"

// Initialize a pins to perform analog and digital output functions

// Adjust analog output pin name to your board spec.

AnalogOut  aout(PA_4);
AnalogIn ain(A0);
DigitalIn B0(D8);
DigitalIn B1(D9);
DigitalIn B2(D10);
uLCD_4DGL uLCD(D1, D0, D2);

int N = 256;
float f[4] = {1,0.5, 0.25, 0.125};
int f_cur = 0;
int f_idx = 0;
void display(){
    uLCD.locate(0, 0);
    for (int i=0;i<4;i++){
        uLCD.color(GREEN);
        if (i==f_cur)
            uLCD.color(BLUE);
        if (i==f_idx)
            uLCD.color(RED);
        uLCD.printf("%.3f   \n", f[i]);
    }
}


float adc_buffer[1024];

int main(void){
    B0.mode(PullNone);
    B1.mode(PullNone);
    B2.mode(PullNone);
    
    int index=0;
    float scale = 3.0/3.3;
    aout = 0;
    
    float T = 1000/f[f_idx];
    uLCD.locate(0, 0);
    uLCD.text_width(2); //4X size text
    uLCD.text_height(2);
    float adc_data=0;
    int adc_cnt = 0;
    int record_flag = 0;
    display();
    while (1) {
        // check button
        if(B0.read()){
            f_cur  = f_cur + 1;
            if(f_cur>3)
                f_cur = 3;
            display();
            // ThisThread::sleep_for(10ms);
        }
        if(B1.read()){
            f_cur = f_cur - 1;
            if(f_cur<0)
                f_cur = 0;
            display();
            // ThisThread::sleep_for(10ms);
        }
        if(B2.read()){
            f_idx = f_cur;
            // freq = f[f_idx];
            aout = 0;
            // printf("Current Frequence : %.1f \n\r", freq);
            display();
            record_flag = 1;
            // ThisThread::sleep_for(1ms);
        }
        // T = 1/ freq
        T = 1000/f[f_idx];
        int T1 = 80 ;
        int T2 = 240 ;
        int slp[4] = {80,40,20,10};
        int slp2[4] = {160,200,220,230};
        int fint[4] = {1,2,4,8};

        if(index < T1/fint[f_idx]){ 
            aout = scale* float(index) / float(slp[f_idx]);
        }
        if(index > T2-(T1/fint[f_idx])){
            aout = (float(T2)-float(index-T2))*float(index) / float(slp[f_idx]);
            //scale*(1- float(index-0.2*T)/0.8/T);
        }
        else{
            aout = scale;
            }
        if(index < 240)
            index = index +1;
        else
            index = 0;
        //
        
        if(record_flag==1){
            adc_buffer[adc_cnt] = ain;
            adc_cnt = adc_cnt +1;
            if(adc_cnt==N){
                for(int i=0;i<N;i++)
                    printf("%d, %f\n", i, adc_buffer[i]);
                printf("---\n");
                record_flag = 0;
                adc_cnt = 0;       
            }   
        }

        ThisThread::sleep_for(1ms);
   }

}