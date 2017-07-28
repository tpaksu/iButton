#include <16F628.h>

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES INTRC_IO              	//Internal RC Osc, no CLKOUT
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES BROWNOUT              	//Reset when brownout detected
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES NOCPD                 	//No EE protection
#FUSES NOLVP                 	//No Low voltage programming

#use delay(clock=4000000)
#define TOUCH_DEVICE   PIN_B3
