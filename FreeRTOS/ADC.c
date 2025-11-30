// ADC.c
#include "xc.h"
#include <stdint.h>
#include "ADC.h"

/*
 * do_ADC()
 * - configures ADC1 to read from Pin 7 (RB3 / AN5):
 *   pin 7 = AN5/C1INA/RP3/RB3  
 * - performs ONE sample/conversion
 * - returns 10-bit result in a 16-bit value
 * - allowed to poll AD1CON1bits.DONE as per lab brief
 */
uint16_t do_ADC(void)
{
    uint16_t result;

    /* make sure the pin is analog and input */
    ANSELBbits.ANSB3 = 1;     /* RB3 = AN5, analog on */
    TRISBbits.TRISB3 = 1;     /* input */

    /* turn ADC off while configuring */
    AD1CON1bits.ADON = 0;

    /* AD1CON1
     * FORM=00 (integer), SSRC=111 (auto convert), ASAM=0 (manual sample)
     */
    AD1CON1 = 0;
    AD1CON1bits.FORM = 0;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.ASAM = 0;

    /* AD1CON2
     * use AVdd/AVss, MUXA, single sample
     */
    AD1CON2 = 0;

    /* AD1CON3
     * ADRC=0 (use system clock)
     * ADCS -> TAD = (ADCS+1)*Tcy; choose a safe slow TAD
     * SAMC -> auto sample time (in TAD)
     */
    AD1CON3 = 0;
    AD1CON3bits.ADCS = 31;    
    AD1CON3bits.SAMC = 10;    /* sample 10 TAD */

    /* Select AN5 (channel 5) on CH0SA */
    AD1CHS = 0;
    AD1CHSbits.CH0SA = 5;     /* AN5 */
    AD1CHSbits.CH0NA = 0;     /* Vref- */

    /* turn ADC on */
    AD1CON1bits.ADON = 1;

    /* start sampling */
    AD1CON1bits.SAMP = 1;

    /* give the ADC some time to acquire the pot voltage */
    {
        volatile uint16_t i;
        for (i = 0; i < 1000; i++)
        {
            Nop();
        }
    }

    /* end sampling -> start conversion */
    AD1CON1bits.SAMP = 0;

    /* wait for conversion to finish */
    while (!AD1CON1bits.DONE) {
        /* polling allowed by assignment */
    }

    result = ADC1BUF0;

    /* clear DONE and turn off if you want to be tidy */
    AD1CON1bits.DONE = 0;
    AD1CON1bits.ADON = 0;

    return result;
}


