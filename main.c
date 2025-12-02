/*
 * File:   main.c
 * Author: 
 * 
 * Jazeb Ahmad Zafar
 * Anas Chowdhury
 * Mayuran Muralitharan
 *
 * Created on November 20th, 2025
 * 
 * Project Description: 
 * This project is a working implementation of a FreeRTOS based finite state machine running on PIC24 hardware using MPlab. 
 * The system acts as a countdown timer controlled by different push buttons, a potentiometer, UART terminal input, and three LEDs with PWM.
 * 
 */
// FSEC
#pragma config BWRP = OFF    //Boot Segment Write-Protect bit->Boot Segment may be written
#pragma config BSS = DISABLED    //Boot Segment Code-Protect Level bits->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GSS = DISABLED    //General Segment Code-Protect Level bits->No Protection (other than GWRP)
#pragma config CWRP = OFF    //Configuration Segment Write-Protect bit->Configuration Segment may be written
#pragma config CSS = DISABLED    //Configuration Segment Code-Protect Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = OFF    //Alternate Interrupt Vector Table bit->Disabled AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->Internal Fast RC (FRC)
#pragma config PLLMODE = PLL96DIV2    //PLL Mode Selection->96 MHz PLL. Oscillator input is divided by 2 (8 MHz input)
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMD = NONE    //Primary Oscillator Mode Select bits->Primary Oscillator disabled
#pragma config OSCIOFCN = ON    //OSC2 Pin Function bit->OSC2 is general purpose digital I/O pin
#pragma config SOSCSEL = OFF    //SOSC Power Selection Configuration bits->Digital (SCLKI) mode
#pragma config PLLSS = PLL_FRC    //PLL Secondary Selection Configuration bit->PLL is fed by the on-chip Fast RC (FRC) oscillator
#pragma config IOL1WAY = ON    //Peripheral pin select configuration bit->Allow only one reconfiguration
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config FWPSA = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config FWDTEN = ON_SWDTEN    //Watchdog Timer Enable bits->WDT Enabled/Disabled (controlled using SWDTEN bit)
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config WDTWIN = WIN25    //Watchdog Timer Window Select bits->WDT Window is 25% of WDT period
#pragma config WDTCMX = WDTCLK    //WDT MUX Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown Out Enable bit->Brown Out Enable Bit
#pragma config LPCFG = OFF    //Low power regulator control->No Retention Sleep
#pragma config DNVPEN = ENABLE    //Downside Voltage Protection Enable bit->Downside protection enabled using ZPBOR when BOR is inactive

// FICD
#pragma config ICS = PGD1    //ICD Communication Channel Select bits->Communicate on PGEC1 and PGED1
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FDEVOPT1
#pragma config ALTCMPI = DISABLE    //Alternate Comparator Input Enable bit->C1INC, C2INC, and C3INC are on their standard pin locations
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config SOSCHP = ON    //SOSC High Power Enable bit (valid only when SOSCSEL = 1->Enable SOSC high power mode (default)
#pragma config ALTI2C1 = ALTI2CEN    //Alternate I2C pin Location->SDA1 and SCL1 on RB9 and RB8


#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "ADC.h"
#include "semphr.h"
#include <xc.h>
#include <stdlib.h>
#include <ctype.h>
// For uint_32t
#include <stdint.h>   

#define TASK_STACK_SIZE 200
#define TASK_PRIORITY 5

// Pin defines
// LED0 is connected to RB6 pin 15 
#define LED0_LAT LATBbits.LATB6
#define LED0_TRIS TRISBbits.TRISB6

// LED1 is connected RB5 pin 14 
#define LED1_LAT LATBbits.LATB5
#define LED1_TRIS TRISBbits.TRISB5

// LED2 RB7 pin 16 pulsing & waiting
#define LED2_LAT LATBbits.LATB7
#define LED2_TRIS TRISBbits.TRISB7

// PB1 is a button with internal pull up 
#define PB1_PORT PORTAbits.RA4
#define PB1_TRIS TRISAbits.TRISA4

// PB2 is a button with internal pull up
#define PB2_PORT PORTBbits.RB8
#define PB2_TRIS TRISBbits.TRISB8

// PB3 also has internal pull up
#define PB3_PORT PORTBbits.RB9
#define PB3_TRIS TRISBbits.TRISB9



// The states are in enums for ease of viewing the state.
typedef enum {
    WAITING_ST = 0,
    STATE_TIME_ENTRY,
    STATE_COUNTDOWN,
    STATE_PAUSED,
    STATE_DONE
} TimerState_t;

// Default state is below
static TimerState_t currentState = WAITING_ST;

// Variables for the pwm counter
volatile uint16_t pwmCounter = 0;
// How many ticks on during the duty for LED2
volatile uint16_t dutyTicks = 0; 

// pulsing parameters for LED2 waiting
volatile int8_t dutyStep = 1;        

//Constants for PWM behaviour
// We set a 5 tick PWM period to avoid any flicker, we used timer 2 @ 1kHz
#define PWM_PERIOD_TICKS 5
#define DUTY_MIN_TICKS 0

// This is the same value as period ticks
#define DUTY_MAX_TICKS PWM_PERIOD_TICKS

// We define slightly slower pulsing here
#define PULSING_STEP_TICKS 150

volatile uint16_t pulseCounter = 0;  

// variable to check if waiting prompt has been shown already
static uint8_t waitingPromptShown = 0;

// variables for the countdown timer
static uint16_t gMinutes = 0;
static uint16_t gSeconds = 0;
static uint8_t  countdownInitialised = 0;

// extern variables from the uart.c file
extern uint8_t received_char;
extern uint8_t RXFlag;

// these are for state timing
static uint16_t doneBlinkCount = 0;
static uint8_t  doneMessageShown = 0;

// LED2 stuff
static uint8_t showExtraInfo = 0;  
// for below -> 1 is blinking mode and 0 is solid  
static uint8_t led2BlinkMode = 1;

// ADC PWM 
static uint16_t led2DutyFromADC = 0;
static uint8_t  led2BlinkPhase  = 1; // start LED2 "on" phase in countdown

// button states and countdown

// internal tick for countdown state
#define COUNTDOWN_TICK_MS       100 
// this is approximately 1.2s long press for abort
#define PB3_LONG_PRESS_MS       1200 
#define PB3_LONG_PRESS_TICKS    (PB3_LONG_PRESS_MS / COUNTDOWN_TICK_MS)

static uint8_t  countdownPaused      = 0;
static uint16_t countdownTickCounter = 0;
static uint16_t pb3HoldTicks         = 0;

// for i -> information mode
static uint16_t lastAdcVal = 0;


// Uart semaphore
SemaphoreHandle_t uart_sem;


void InitTimer2ForPWM(void)
{
    /* 
     * This function is setting up timer 2 for LED
     */
    
    // Turn off Timer2
    T2CONbits.TON = 0;      
    T2CONbits.TCS = 0;
    // Setting the prescaler to 1:8, from the datasheet
    T2CONbits.TCKPS = 0b01;
    
    // Clear timer 2
    TMR2 = 0;
    // Value for 1ms period, uses -> Fcy = 16MHz, prescaler = 8)
    PR2 = 249;              

    // Clear and enable the interrupts
    IFS0bits.T2IF = 0;//Clear
    IEC0bits.T2IE = 1; //Enable      
    IPC1bits.T2IP = 3;//Priority
    
    //Turn on
    T2CONbits.TON = 1;
}

// FreeRTOS requirement due to IDLE 1 define up above
void vApplicationIdleHook( void )
{
}

// Same as above, required by FreeRTOS
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

// FreeRTOS task prototypes
void vWaitingTask(void *pvParameters);
void vTimeEntryTask(void *pvParameters);
void vCountdownTask(void *pvParameters);
void vDoneTask(void *pvParameters);

// Interrupt function
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{   
    // Flag clear
    IFS0bits.T2IF = 0;
    
    // LED2 always generates pwm for LED2 variable brightness and pulsing
    if (pwmCounter < dutyTicks)
    {   
        LED2_LAT = 1;   
    }
    else
    {
        LED2_LAT = 0;
    }

    pwmCounter++;
    if (pwmCounter >= PWM_PERIOD_TICKS)
    {
        pwmCounter = 0;
    }

    // pulse LED2 regularly when waiting
    if (currentState == WAITING_ST)
    {
        // Pulsing code
        pulseCounter++;
        if (pulseCounter >= PULSING_STEP_TICKS)
        {
            pulseCounter = 0;

            // Adjust duty by one (+/-)
            dutyTicks += dutyStep;

            // Reverse the duty tick direction when it hits the limits
            if (dutyTicks >= DUTY_MAX_TICKS)
            {
                dutyTicks = DUTY_MAX_TICKS;
                // dimming is on
                dutyStep = -1;  
            }
            else if (dutyTicks <= DUTY_MIN_TICKS)
            {
                dutyTicks = DUTY_MIN_TICKS;
                // brightening is on
                dutyStep = 1;  
            }
        }
    }
}

// Print command for the time
static void PrintTimeUART(uint16_t minutes, uint16_t seconds)
{
    // Protect UART with the semaphore :)
    xSemaphoreTake(uart_sem, portMAX_DELAY);

    // Show the time 
    Disp2String("\n\rTime remaining: ");

    // Tens and ones for the minutes
    XmitUART2((minutes / 10) + '0', 1);
    XmitUART2((minutes % 10) + '0', 1);

    // seperater using colon
    XmitUART2(':', 1);

    // tens and ones for seconds
    XmitUART2((seconds / 10) + '0', 1);
    XmitUART2((seconds % 10) + '0', 1);
    
    // release semaphore
    xSemaphoreGive(uart_sem);
}



// helper to print a small uint as decimal
// Manual conversion for numbers to text (can't do printf)
static void PrintUIntDec(uint16_t val)
{
    // final output buffer 5 digits and null
    char buf[6];
    int idx = 0;

    // There is a special case for zero just print 0
    if (val == 0)
    {
        buf[idx++] = '0';
    }
    else
    {
        // store the digits in reverse order temporarily
        char tmp[6];
        int t = 0;
        
        // take out digits from least to most significant
        while (val > 0 && t < 5)
        {
            // Ascii conversion
            tmp[t++] = (val % 10) + '0';
            val /= 10;
        }
        // Reverse back into the output buffer
        while (t > 0)
        {
            buf[idx++] = tmp[--t];
        }
    }
    // null string for ending
    buf[idx] = '\0';
    
    // Send each char over uart individually 
    for (int i = 0; buf[i] != '\0'; i++)
    {
        XmitUART2(buf[i], 1);
    }
}


// Waiting task
void vWaitingTask(void *pvParameters)
{
    (void) pvParameters;

    // Print banner at startup
    static uint8_t bannerPrinted = 0;
    static uint8_t oncePrintedPB1Debug = 0;

    for (;;)
    {
        if (currentState != WAITING_ST)
        {
            // Just a regular yield 
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        // Initial banner printing
        if (!bannerPrinted)
        {
            xSemaphoreTake(uart_sem, portMAX_DELAY);
            Disp2String("\n----------------------Fancy Timer Project-----------------------\n\r");
            Disp2String("\nAuthors: Jazeb, Mayuran and Anas (Group 13)\n\n\r");
            Disp2String("The Current State of the FSM is: WAITING. LED2 should be pulsing\n");
            Disp2String("To move forward, please press PB1 to begin setting a countdown time.\n");
            xSemaphoreGive(uart_sem);

            bannerPrinted = 1;
        }

        // LEDs for WAITING state
        LED0_LAT = 0;
        LED1_LAT = 0;
        // LED2 pulsing is handled in Timer2 interrupt service routine

        // Show the waiting message only once each time we return to WAITING
        if (!waitingPromptShown)
        {
            xSemaphoreTake(uart_sem, portMAX_DELAY);
            Disp2String("\n\r[WAITING] Press PB1 to begin setting a countdown time.\n\r");
            xSemaphoreGive(uart_sem);

            waitingPromptShown = 1;
        }

        // Debug print - only for button testing
        /*
        if (!oncePrintedPB1Debug)
        {
            xSemaphoreTake(uart_sem, portMAX_DELAY);
            if (PB1_PORT)
                Disp2String("\n\r[DEBUG] PB1 at reset: 1 (HIGH)\n\r");
            else
                Disp2String("\n\r[DEBUG] PB1 at reset: 0 (LOW)\n\r");
            xSemaphoreGive(uart_sem);

            oncePrintedPB1Debug = 1;
        }
        */

        // PB1 click detection simple debounce
        if (PB1_PORT == 0)  // button pressed is active low
        {
            vTaskDelay(pdMS_TO_TICKS(50));  // debounce time

            if (PB1_PORT == 0)
            {
                xSemaphoreTake(uart_sem, portMAX_DELAY);
                Disp2String("\n\r[WAITING] PB1 press detected, moving to TIME_ENTRY.\n\r");
                xSemaphoreGive(uart_sem);

                // wait for release to prevent re input of button
                while (PB1_PORT == 0)
                {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                waitingPromptShown = 0;   // so the waiting prompt shows again next time
                currentState = STATE_TIME_ENTRY;
            }
        }

        // Run quickly for responsiveness
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


// Time Entry State
void vTimeEntryTask(void *pvParameters)
{
    (void) pvParameters;

    // Input characters buffer 
    char inputBuf[8];

    for (;;)
    {
        if (currentState != STATE_TIME_ENTRY)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        // Stop LED2 pulsing when entering time
        dutyTicks = 0;
        LED2_LAT  = 0;

        LED0_LAT = 0;
        LED1_LAT = 0;
        
        // this is so the WAITING message runs again next time we enter that state
        waitingPromptShown = 0; 

        xSemaphoreTake(uart_sem, portMAX_DELAY);
        Disp2String("\n\r[TIME ENTRY] Please enter time as MMSS (e.g., 0130 for 1min 30s), then press ENTER:\n\r> ");
        xSemaphoreGive(uart_sem);

        // Blocking receives and then echoes characters back
        RecvUart(inputBuf, sizeof(inputBuf));
        inputBuf[sizeof(inputBuf)-1] = '\0';

        // This is to extract digits only 
        char digits[5] = {0};
        int di = 0;
        for (int i = 0; inputBuf[i] != '\0' && di < 4; i++)
        {
            if (isdigit((unsigned char)inputBuf[i]))
            {
                digits[di++] = inputBuf[i];
            }
        }

        int mm = 0;
        int ss = 0;

        if (di == 4)
        {
            // MMSS
            mm = (digits[0]-'0')*10 + (digits[1]-'0');
            ss = (digits[2]-'0')*10 + (digits[3]-'0');
        }
        else if (di == 3)
        {
            // M SS
            mm = (digits[0]-'0');
            ss = (digits[1]-'0')*10 + (digits[2]-'0');
        }
        else if (di == 2)
        {
            // treat as seconds only
            mm = 0;
            ss = (digits[0]-'0')*10 + (digits[1]-'0');
        }
        else
        {
            // if invalid -> default 00:10
            mm = 0;
            ss = 10;
        }
        // 60 seconds in a minute
        if (ss > 59) ss = 59;

        gMinutes = (uint16_t)mm;
        gSeconds = (uint16_t)ss;

        xSemaphoreTake(uart_sem, portMAX_DELAY);
        Disp2String("\n\r[TIME ENTRY] Time set.\n\r");
        Disp2String("[TIME ENTRY] Click PB2 and PB3 together to start.\n\r");
        Disp2String("[TIME ENTRY] Long press PB2+PB3 to reset and re-enter time.\n\r");
        xSemaphoreGive(uart_sem);

        // Wait here for PB2+PB3 short or long press 
        uint16_t comboHoldTicks = 0;
        uint8_t  comboActive    = 0;
        // Approx 1s long press
        const uint16_t COMBO_LONG_PRESS_MS    = 1000; 
        const uint16_t COMBO_TICK_MS          = 20;
        const uint16_t COMBO_LONG_PRESS_TICKS = (COMBO_LONG_PRESS_MS / COMBO_TICK_MS);

        for (;;)
        {
            // If other tasks changed the state then break
            if (currentState != STATE_TIME_ENTRY)
            {
                break;
            }
            // 1 = released, 0 = pressed. (pullup button )
            uint8_t p2 = PB2_PORT; 
            uint8_t p3 = PB3_PORT;

            if ((p2 == 0) && (p3 == 0))
            {
                // both pressed
                if (!comboActive)
                {
                    comboActive    = 1;
                    comboHoldTicks = 0;
                }
                else if (comboHoldTicks < 0xFFFF)
                {
                    comboHoldTicks++;
                }
            }
            else
            {
                if (comboActive)
                {
                    // both were pressed and atleast one is released
                    if (comboHoldTicks >= COMBO_LONG_PRESS_TICKS)
                    {
                        // Long press is to reset timer
                        xSemaphoreTake(uart_sem, portMAX_DELAY);
                        Disp2String("\n\r[TIME ENTRY] Long press PB2+PB3 detected. Resetting time.\n\r");
                        xSemaphoreGive(uart_sem);

                        gMinutes = 0;
                        gSeconds = 0;
                        // stay in STATE_TIME_ENTRY, the outer loop will ask for it again 
                    }
                    else if (comboHoldTicks > 0)
                    {
                        // Short click starts the countdown
                        xSemaphoreTake(uart_sem, portMAX_DELAY);
                        Disp2String("\n\r[TIME ENTRY] Starting countdown.\n\r");
                        xSemaphoreGive(uart_sem);

                        countdownInitialised = 0;
                        currentState         = STATE_COUNTDOWN;
                    }

                    break;  // exit inner loop 
                }

                // no active combo
                comboActive    = 0;
                comboHoldTicks = 0;
            }

            vTaskDelay(pdMS_TO_TICKS(COMBO_TICK_MS));
        }

        // Outer for(;;) repeats and if state is in TIME_ENTRY we re ask, if it is in the COUNTDOWN we just idle
    }
}


// Countdown task (periodic task which means it repeats)
void vCountdownTask(void *pvParameters)
{
    (void) pvParameters;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        if (currentState != STATE_COUNTDOWN)
        {
            // when we leave COUNTDOWN, ensure next attempt goes back to initialization 
            countdownInitialised = 0;
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        // Initialize once when we first enter COUNTDOWN
        if (!countdownInitialised)
        {
            LED0_LAT = 0;
            LED1_LAT = 1;  // start with LED1 on for 1 Hz blink
            LED2_LAT = 0;  // LED2 brightness via dutyTicks + ADC

            // reset the pulsing state for LED2 now LED2 will be driven by ADC logic
            pulseCounter = 0;
            dutyStep     = 1;

            xLastWakeTime         = xTaskGetTickCount();
            countdownTickCounter  = 0;
            countdownPaused       = 0;
            pb3HoldTicks          = 0;
            // start LED2 in the on phase
            led2BlinkPhase        = 1;
            // default is blinking
            led2BlinkMode         = 1;
            // start with simple time display
            showExtraInfo         = 0;   

            xSemaphoreTake(uart_sem, portMAX_DELAY);
            Disp2String("\n\r[COUNTDOWN] Countdown started.\n\r");
            Disp2String("[COUNTDOWN] Click PB3 to pause/resume. Long press PB3 to abort.\n\r");
            Disp2String("[COUNTDOWN] Type 'i' to toggle extra info, 'b' to toggle LED2 blink/solid.\n\r");
            xSemaphoreGive(uart_sem);

            countdownInitialised = 1;
        }

        // Periodic tick 100 ms
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(COUNTDOWN_TICK_MS));

        // PB3 is for pause/resume (short click) and abort is a (long press) 
        {   
            
            uint8_t pb3 = PB3_PORT;

            if (pb3 == 0)
            {
                // button held down
                if (pb3HoldTicks < 0xFFFF)
                {
                    pb3HoldTicks++;
                }
            }
            else
            {
                // button released
                if (pb3HoldTicks > 0)
                {
                    if (pb3HoldTicks >= PB3_LONG_PRESS_TICKS)
                    {
                        // Long press sends it back to 0:00 and then it will go to DONE
                        gMinutes = 0;
                        gSeconds = 0;

                        xSemaphoreTake(uart_sem, portMAX_DELAY);
                        Disp2String("\n\r[COUNTDOWN] Long press PB3 detected. Aborting timer to 00:00.\n\r");
                        xSemaphoreGive(uart_sem);

                        countdownInitialised = 0;
                        doneBlinkCount       = 0;
                        doneMessageShown     = 0;
                        currentState         = STATE_DONE;

                        pb3HoldTicks = 0;
                        continue; //in the next one it will be state done and next logic
                    }
                    else
                    {
                        // Short click is for toggle pause/resume
                        countdownPaused ^= 1;

                        xSemaphoreTake(uart_sem, portMAX_DELAY);
                        if (countdownPaused)
                        {
                            Disp2String("\n\r[COUNTDOWN] Paused.\n\r");
                        }
                        else
                        {
                            Disp2String("\n\r[COUNTDOWN] Resumed.\n\r");
                        }
                        xSemaphoreGive(uart_sem);
                    }

                    pb3HoldTicks = 0;
                }
            }
        }

        // Handle the i and b uart commands
        if (RXFlag)
        {
            char c = received_char;
            // clear this flag so we can see the next char
            RXFlag = 0;     

            if (c == 'i')
            {
                // turn on variable that shows extra information
                showExtraInfo ^= 1;
            }
            else if (c == 'b')
            {
                // toggle LED2 mode either blink or solid
                led2BlinkMode ^= 1;

                xSemaphoreTake(uart_sem, portMAX_DELAY);
                if (led2BlinkMode)
                {
                    Disp2String("\n\r[COUNTDOWN] LED2 set to BLINK mode.\n\r");
                }
                else
                {
                    Disp2String("\n\r[COUNTDOWN] LED2 set to SOLID mode.\n\r");
                }
                xSemaphoreGive(uart_sem);
            }
        }

        // Here ADC reads 
        {
            uint16_t adcVal = do_ADC();   // 0..1023 from AN5
            lastAdcVal = adcVal;

            // Map ADC -> duty in [0, DUTY_MAX_TICKS]
            led2DutyFromADC = (uint32_t)adcVal * DUTY_MAX_TICKS / 1023u;
            if (led2DutyFromADC > DUTY_MAX_TICKS)
            {
                led2DutyFromADC = DUTY_MAX_TICKS;
            }

            // Use current blink phase to decide ON/OFF for LED2
            if (led2BlinkMode)
            {
                if (led2BlinkPhase)
                {
                    dutyTicks = led2DutyFromADC;   // LED2 on at chosen brightness
                }
                else
                {
                    dutyTicks = 0;                 // LED2 off
                }
            }
            else
            {
                // Solid: LED2 always at chosen brightness
                dutyTicks = led2DutyFromADC;
            }
        }

        // Increments countdown tick counter
        countdownTickCounter++;
        if (countdownTickCounter >= (1000 / COUNTDOWN_TICK_MS))
        {
            countdownTickCounter = 0;

            // Only decrement and blink when not paused
            if (!countdownPaused)
            {
                // Decrement time
                if (gMinutes == 0 && gSeconds == 0)
                {
                    // already at zero, go to DONE
                    currentState     = STATE_DONE;
                    doneBlinkCount   = 0;
                    doneMessageShown = 0;
                }
                else
                {
                    if (gSeconds == 0)
                    {
                        if (gMinutes > 0)
                        {
                            gMinutes--;
                            gSeconds = 59;
                        }
                    }
                    else
                    {
                        gSeconds--;
                    }

                    // Blink LED1 at 1 Hz approximately (slightly faster) toggle every second
                    LED1_LAT ^= 1;  // 1s on / 1s off

                    // LED2 blink phase toggle once each second if in blink mode
                    if (led2BlinkMode)
                    {
                        led2BlinkPhase ^= 1;
                    }
                }
            }

            // Print time (simple/extended)
            if (!showExtraInfo)
            {
                // simple time view
                PrintTimeUART(gMinutes, gSeconds);
            }
            else
            {
                // extended view with time, ADC, duty and the mode
                xSemaphoreTake(uart_sem, portMAX_DELAY);
                Disp2String("\n\rTime remaining (extended): ");
                xSemaphoreGive(uart_sem);

                PrintTimeUART(gMinutes, gSeconds);

                xSemaphoreTake(uart_sem, portMAX_DELAY);
                Disp2String(" | ADC = ");
                PrintUIntDec(lastAdcVal);

                Disp2String(" | LED2 dutyTicks = ");
                PrintUIntDec(led2DutyFromADC);

                Disp2String(" | LED2 mode = ");
                if (led2BlinkMode)
                {
                    Disp2String("BLINK");
                }
                else
                {
                    Disp2String("SOLID");
                }

                xSemaphoreGive(uart_sem);
            }

            // If it has reached 0
            if (!countdownPaused && gMinutes == 0 && gSeconds == 0)
            {
                currentState     = STATE_DONE;
                doneBlinkCount   = 0;
                doneMessageShown = 0;
            }
        }
    }
}


// Done state
void vDoneTask(void *pvParameters)
{
    (void) pvParameters;

    for (;;)
    {
        if (currentState != STATE_DONE)
        {
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }

        // After the timer has finished, the terminal messages that the countdown is done,
        // and LED0 and LED1 blink rapidly in an alternating fashion.
        // LED2 should remain solidly on. After 5 seconds it will return to WAITING.

        if (!doneMessageShown)
        {
            xSemaphoreTake(uart_sem, portMAX_DELAY);
            Disp2String("\n\r[DONE] Countdown complete! Timer reached 00:00.\n\r");
            xSemaphoreGive(uart_sem);

            doneMessageShown = 1;
            doneBlinkCount   = 0;

            LED0_LAT = 1;
            LED1_LAT = 0;

            // LED2 is solid, brightness from ADC via the PWM
            led2BlinkMode = 0;  // sets it to solid mode
        }

        // Alternate LED0 and LED1 quickly approx 100ms period
        LED0_LAT ^= 1;
        LED1_LAT = !LED0_LAT;

        // LED2 brightness still controlled by potentiometer when solid during this phase
        {
            uint16_t adcVal = do_ADC();
            lastAdcVal = adcVal;
            led2DutyFromADC = (uint32_t)adcVal * DUTY_MAX_TICKS / 1023u;
            if (led2DutyFromADC > DUTY_MAX_TICKS)
            {
                led2DutyFromADC = DUTY_MAX_TICKS;
            }

            // In DONE, LED2 should be solid at the ADC brightness
            dutyTicks = led2DutyFromADC;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
        doneBlinkCount++;

        // formula for the time is 100ms * 50 = 5s
        if (doneBlinkCount >= 50)
        {
            // Turn LEDs off and go back to waiting
            LED0_LAT = 0;
            LED1_LAT = 0;
            LED2_LAT = 0;

            gMinutes = 0;
            gSeconds = 0;
            countdownInitialised = 0;

            // reset pulsing variables for waiting state for LED2
            dutyTicks    = 0;
            dutyStep     = 1;
            pulseCounter = 0;

            waitingPromptShown = 0;
            currentState       = WAITING_ST;
        }
    }
}



void prvHardwareSetup(void)
{
    // LED pins as outputs
    LED0_TRIS = 0;
    LED1_TRIS = 0;
    LED2_TRIS = 0;

    LED0_LAT = 0;
    LED1_LAT = 0;
    LED2_LAT = 0;

    
    InitUART2();
    
    // PB1 RA4 as input
    PB1_TRIS = 1;         

    // RA4 as a pull-up
    CNPUAbits.CNPUA4 = 1;
    
    // PB2 and PB3
    PB2_TRIS = 1;
    PB3_TRIS = 1;
    
    // here just double check that PB2/PB3 pins are DIGITAL, not analog (sometimes goes analog)
    ANSELBbits.ANSB9 = 0;   // PB3 on RB9 as digital input

    // enable pull ups on RB8 and RB9
    CNPUBbits.CNPUB8 = 1;   // RB8 pull-up
    CNPUBbits.CNPUB9 = 1;   // RB9 pull-up


    // Initialize the timer 2 for LED pulsing and PWM
    InitTimer2ForPWM();
    
}

int main(void) {
    
    prvHardwareSetup();

    uart_sem = xSemaphoreCreateMutex();
    
    // FSM initialization for ALL variables 
    currentState          = WAITING_ST;
    waitingPromptShown    = 0;
    countdownInitialised  = 0;
    doneBlinkCount        = 0;
    doneMessageShown      = 0;
    gMinutes              = 0;
    gSeconds              = 0;

    pwmCounter            = 0;
    dutyTicks             = 0;
    dutyStep              = 1;
    pulseCounter          = 0;

    showExtraInfo         = 0;
    led2BlinkMode         = 1; //blink is set on by default here
    led2DutyFromADC       = 0;
    led2BlinkPhase        = 1;

    countdownPaused       = 0;
    countdownTickCounter  = 0;
    pb3HoldTicks          = 0;
    lastAdcVal            = 0;

    // Using multiple tasks for good FreeRTOS implementation
    // WAITING & PB1 
    xTaskCreate( vWaitingTask, "WaitTask", TASK_STACK_SIZE, NULL, 2, NULL);

    // TIME ENTRY using UART with the PB2+PB3 combo (aperiodic)
    xTaskCreate( vTimeEntryTask, "TimeEntryTask", TASK_STACK_SIZE, NULL, 2, NULL);

    // COUNTDOWN has the core timing with the ADC, PB3 pause/abort and "i"/"b" its a periodic function
    xTaskCreate( vCountdownTask, "CountdownTask", TASK_STACK_SIZE, NULL, 3, NULL);

    // DONE state has LED2 as solid via the ADC and a timeout back to WAITING
    xTaskCreate( vDoneTask, "DoneTask", TASK_STACK_SIZE, NULL, 2, NULL);

    vTaskStartScheduler();
    
    for(;;);
}
