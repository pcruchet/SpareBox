

#include "stm32l476.h"

attachInterrupt(digitalPinToInterrupt(USER_BUTTON), ISR, FALLING);  
DigitalOut myled(LED1);
 
float delay = 1.0; // 1 sec
 
void pressed()
{
    if (delay == 1.0)
        delay = 0.2; // 200 ms
    else
        delay = 1.0; // 1 sec
}
 
int main()
{
    mybutton.fall(&pressed);
    while (1) {
        myled = !myled;
        wait(delay);
    }
}
