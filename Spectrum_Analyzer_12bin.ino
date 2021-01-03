/*
 * Arduino Audio Visualizer (Spectrum Analyzer)
 * 
 * Brian Wang
 * 30 December 2020
 * MIT Voltage
 * 
 * Code Adapted from learnelectronics
 * 
 * Fix_FFT library available @ https://github.com/kosme/arduinoFFT
 * 
 * Turning potentiometers CCW while header pins are on the bottom provides max gain
 */


#include "fix_fft.h"                                  //library to perfom the Fast Fourier Transform

char im[128], input[128];                              //variables for the FFT
char x = 0, ylim = 60;                                //variables for drawing the graphics
int i = 0, val;                                       //counters

//led pin definitions
int a_red = 13;
int b_red = 12;
int c_red = 11;
int a_yellow = 10;
int b_yellow = 9;
int c_yellow = 8;
int a_green = 7;
int b_green = 6;
int c_green = 5;
int a_blue = 4;
int b_blue = 3;
int c_blue = 2;

//4 bin summation variables
int a_rSum, b_rSum, c_rSum, a_ySum, b_ySum, c_ySum, a_gSum, b_gSum, c_gSum, a_bSum, b_bSum, c_bSum;

//ideal threshold around 6 but may differ depending on music genre
int threshold = 6;


void setup()
{
  Serial.begin(9600);                                 //serial comms for debuging
  analogReference(DEFAULT);                           // Use default (5v) aref voltage.
  pinMode(a_red, OUTPUT);                             //define all LEDs as outputs
  pinMode(b_red, OUTPUT);
  pinMode(c_red, OUTPUT);
  
  pinMode(a_yellow, OUTPUT);
  pinMode(b_yellow, OUTPUT);
  pinMode(c_yellow, OUTPUT);
  
  pinMode(a_green, OUTPUT);
  pinMode(b_green, OUTPUT);
  pinMode(c_green, OUTPUT);
  
  pinMode(a_blue, OUTPUT);
  pinMode(b_blue, OUTPUT);
  pinMode(c_blue, OUTPUT);
};

void loop()
{
  int min=1024, max=0;                                //set minumum & maximum ADC values
  for (i = 0; i < 128; i++) {                         //take 128 samples
    val = analogRead(A0);                             //get audio from Analog 0
    input[i] = val / 4 - 128;                         //each element of array is val/4-128
    //Serial.println(val);
    im[i] = 0;                                        //
    if(val>max) max=val;                              //capture maximum level
    if(val<min) min=val;                              //capture minimum level
  };
    
  fix_fft(input, im, 7, 0);                            //perform the FFT on data
 
  a_rSum = 0;
  b_rSum = 0;
  c_rSum = 0;
  
  a_ySum = 0;
  b_ySum = 0;
  c_ySum = 0;
  
  a_gSum = 0;
  b_gSum = 0;
  c_gSum = 0;
  
  a_bSum = 0;
  b_bSum = 0;
  c_bSum = 0;

  //Split 64 frequencies down to 12 bins by summation to find average
  for (i = 0; i<5; i++){
     a_rSum += abs(input[i]);   //utilizes magnitude of FFT values                                         
  }
  for (i = 5; i<10; i++){
     b_rSum += abs(input[i]);                                        
  }
  for (i = 10; i<16; i++){
     c_rSum += abs(input[i]);                                           
  }
  
  for (i = 16; i<21; i++){
     a_ySum += abs(input[i]);                                            
  }
  for (i = 21; i<26; i++){
     b_ySum += abs(input[i]);                                            
  }
  for (i = 26; i<32; i++){
     c_ySum += abs(input[i]);                                            
  }
  
  for (i = 32; i<37; i++){
     a_gSum += abs(input[i]);                                            
  }
  for (i = 37; i<42; i++){
     b_gSum += abs(input[i]);                                            
  }
  for (i = 42; i<48; i++){
     c_gSum += abs(input[i]);                                            
  }
  
  for (i = 48; i<53; i++){
     a_bSum += abs(input[i]);                                            
  }
  for (i = 53; i<58; i++){
     b_bSum += abs(input[i]);                                            
  }
  for (i = 58; i<65; i++){
     c_bSum += abs(input[i]);                                            
  }

  //if average within bin is past threshold, corresponding light is on
  light_threshold(a_rSum, threshold, a_red);
  light_threshold(b_rSum, threshold, b_red);
  light_threshold(c_rSum, threshold, c_red);

  light_threshold(a_ySum, threshold, a_yellow);
  light_threshold(b_ySum, threshold, b_yellow);
  light_threshold(c_ySum, threshold, c_yellow);

  light_threshold(a_gSum, threshold, a_green);
  light_threshold(b_gSum, threshold, b_green);
  light_threshold(c_gSum, threshold, c_green);

  light_threshold(a_bSum, threshold, a_blue);
  light_threshold(b_bSum, threshold, b_blue);
  light_threshold(c_bSum, threshold, c_blue);


  //Serial.println("-----");
};

void light_threshold(int sum, int threshold, int led){
  if(sum/5 > threshold){
    digitalWrite(led, 1);
  }else{
    digitalWrite(led, 0);
  }
}
