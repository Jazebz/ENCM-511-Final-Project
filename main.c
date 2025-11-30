/*
 * File:   main.c
 * Author: ENCM 511
 *
 * Created on Oct 22, 2025
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
#include <stdint.h>   // for uint32_t

#define TASK_STACK_SIZE 200
#define TASK_PRIORITY 5

// Pin defines (per your mapping)
// LED0: RB6 pin 15 (post-countdown alternating with LED1)
#define LED0_LAT   LATBbits.LATB6
#define LED0_TRIS  TRISBbits.TRISB6

// LED1: RB5 pin 14 (COUNTDOWN blink, DONE alternating)
#define LED1_LAT   LATBbits.LATB5
#define LED1_TRIS  TRISBbits.TRISB5

// LED2: RB7 pin 16 (WAITING pulsing, variable brightness via ADC)
#define LED2_LAT   LATBbits.LATB7
#define LED2_TRIS  TRISBbits.TRISB7

// PB1 pull up button
#define PB1_PORT   PORTAbits.RA4
#define PB1_TRIS   TRISAbits.TRISA4

// PB2
#define PB2_PORT   PORTBbits.RB8
#define PB2_TRIS   TRISBbits.TRISB8

// PB3 (wired to RB9 now)
#define PB3_PORT   PORTBbits.RB9
#define PB3_TRIS   TRISBbits.TRISB9



// Puts the states in enums to make it non changeable
typedef enum {
    WAITING_ST = 0,
    STATE_TIME_ENTRY,
    STATE_COUNTDOWN,
    STATE_PAUSED,
    STATE_DONE
} TimerState_t;

// Default state is set below
static TimerState_t currentState = WAITING_ST;

// ---------- Timer2 led pulsing ----------
volatile uint16_t pwmCounter = 0;
// How many ticks on during the duty (percentage of total) for LED2
volatile uint16_t dutyTicks = 0; 

// pulsing parameters (used for LED2 in WAITING)
volatile int8_t   dutyStep = 1;        

// Constants for PWM behaviour
// 5-tick PWM period to avoid visible flicker (Timer2 @ 1 kHz)
#define PWM_PERIOD_TICKS 5
#define DUTY_MIN_TICKS 0
// Same as period ticks; can change for max brightness or separation of timing
#define DUTY_MAX_TICKS PWM_PERIOD_TICKS

// How often does brightness change (ms steps via Timer2 1ms tick)
// Slightly slower pulsing
#define PULSING_STEP_TICKS 150

volatile uint16_t pulseCounter = 0;  

// So the waiting prompt is shown only once
static uint8_t waitingPromptShown = 0;

// Countdown time variables
static uint16_t gMinutes = 0;
static uint16_t gSeconds = 0;
static uint8_t  countdownInitialised = 0;

// from uart.c 
extern uint8_t received_char;
extern uint8_t RXFlag;

// State timing
static uint16_t doneBlinkCount = 0;
static uint8_t  doneMessageShown = 0;

// LED2 stuff
static uint8_t showExtraInfo = 0;  
static uint8_t led2BlinkMode = 1;    // 1 = blink, 0 = solid

// ADC Pwm 
static uint16_t led2DutyFromADC = 0;
static uint8_t  led2BlinkPhase  = 1; // start LED2 "on" phase in countdown

// countdown and button states 
#define COUNTDOWN_TICK_MS       100 // internal tick for countdown state
#define PB3_LONG_PRESS_MS       1200 // ~1.2s long press for abort
#define PB3_LONG_PRESS_TICKS    (PB3_LONG_PRESS_MS / COUNTDOWN_TICK_MS)

static uint8_t  countdownPaused      = 0;
static uint16_t countdownTickCounter = 0;
static uint16_t pb3HoldTicks         = 0;

// for i (extended info mode)
static uint16_t lastAdcVal = 0;



SemaphoreHandle_t uart_sem;

// PB1 checker (still available if you want to use it later)
static uint8_t CheckPB1Click(void)
{
    // Simple software sample this every call
    // click = low, release = high

    static uint8_t stable = 1;
    static uint8_t lastStable = 1;
    // to hold and check value
    static uint8_t cnt = 0;

    uint8_t raw = PB1_PORT;

    if (raw == stable)
    {
        cnt = 0;
    }
    else
    {
        cnt++;
        // 30 ms
        if (cnt >= 3)
        {
            lastStable = stable;
            stable     = raw;
            cnt        = 0;
        }
    }

    // check release
    if (lastStable == 0 && stable == 1)
    {
        // returns true
        return 1;
    }

    return 0;
}


void InitTimer2ForPWM(void)
{
    /* 
     * Timer setup for the LED pulsing
     */
    
    // Turn off Timer2
    T2CONbits.TON = 0;      
    T2CONbits.TCS = 0;
    // Set the prescaler to 1:8 per datasheet
    T2CONbits.TCKPS = 0b01;
    
    // Clear timer
    TMR2 = 0;
    // Value for 1ms period (Fcy=16MHz, prescale=8)
    PR2 = 249;              

    // Clear and enable the interrupts
    IFS0bits.T2IF = 0; //Clear
    IEC0bits.T2IE = 1; //Enable      
    IPC1bits.T2IP = 3; //Priority
    
    //Turn on
    T2CONbits.TON = 1;
}


void vApplicationIdleHook( void )
{
}
/*-----------------------------------------------------------*/

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

static int counter = 0;

/* Test task for reference
void vTask1(void *pvParameters)
{
    for(;;)
    {
        xSemaphoreTake(uart_sem, portMAX_DELAY);
        Disp2String("hello from Task 1\n\r");
        xSemaphoreGive(uart_sem);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
*/

void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
{   
    // Flag clear
    IFS0bits.T2IF = 0;
    
    // LED2: always gen pwm for LED2 (variable brightness or pulsing)
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

    // auto pulse LED2 when waiting
    if (currentState == WAITING_ST)
    {
        // Pulsing 
        pulseCounter++;
        if (pulseCounter >= PULSING_STEP_TICKS)
        {
            pulseCounter = 0;

            // Adjust duty by one step
            dutyTicks += dutyStep;

            // Reverse at bounds
            if (dutyTicks >= DUTY_MAX_TICKS)
            {
                dutyTicks = DUTY_MAX_TICKS;
                // dimming
                dutyStep = -1;  
            }
            else if (dutyTicks <= DUTY_MIN_TICKS)
            {
                dutyTicks = DUTY_MIN_TICKS;
                // brightening
                dutyStep = 1;  
            }
        }
    }
}

// Print command for time
static void PrintTimeUART(uint16_t minutes, uint16_t seconds)
{
    // Protect UART with the sephamore :)
    xSemaphoreTake(uart_sem, portMAX_DELAY);

    // Show time 
    Disp2String("\n\rTime remaining: ");

    // Minutes tens and ones
    XmitUART2((minutes / 10) + '0', 1);
    XmitUART2((minutes % 10) + '0', 1);

    // Colon separator
    XmitUART2(':', 1);

    // Seconds tens and ones
    XmitUART2((seconds / 10) + '0', 1);
    XmitUART2((seconds % 10) + '0', 1);

    xSemaphoreGive(uart_sem);
}



// helper to print a small uint as decimal (no stdio)
static void PrintUIntDec(uint16_t val)
{
    char buf[6];
    int idx = 0;

    if (val == 0)
    {
        buf[idx++] = '0';
    }
    else
    {
        char tmp[6];
        int t = 0;
        while (val > 0 && t < 5)
        {
            tmp[t++] = (val % 10) + '0';
            val /= 10;
        }
        while (t > 0)
        {
            buf[idx++] = tmp[--t];
        }
    }
    buf[idx] = '\0';

    for (int i = 0; buf[i] != '\0'; i++)
    {
        XmitUART2(buf[i], 1);
    }
}



void vMainTimerTask(void *pvParameters)
{
    // Default state is the waiting state
    currentState = WAITING_ST;
    waitingPromptShown = 0;
    countdownInitialised = 0;
    doneBlinkCount = 0;
    doneMessageShown = 0;
    gMinutes = 0;
    gSeconds = 0;

    // One time UART print message
    xSemaphoreTake(uart_sem, portMAX_DELAY);
    Disp2String("\n----------------------Fancy Timer Project-----------------------\n\r");
    Disp2String("\nAuthors: Jazeb, Mayuran and Anas (Group 13)\n\n\r");
    Disp2String("The Current State of the FSM is: WAITING. LED2 should be pulsing\n");
    Disp2String("To move forward, please press PB1 to begin setting a countdown time.\n");
    xSemaphoreGive(uart_sem);
    
    // Input characters
    char inputBuf[8];

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        // Switch statement for the different states (FSM)
        switch (currentState)
        {
            case WAITING_ST:
            {
                // LED2 pulsing via Timer2 ISR (dutyTicks auto-updated)
                // LEDs: LED0 and LED1 off in WAITING
                LED0_LAT = 0;
                LED1_LAT = 0;

                // Show the waiting message only once
                if (!waitingPromptShown)
                {
                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                    Disp2String("\n\r[WAITING] Press PB1 to begin setting a countdown time.\n\r");
                    xSemaphoreGive(uart_sem);

                    waitingPromptShown = 1;
                }

                static uint8_t oncePrinted = 0;
                if (!oncePrinted)
                {
                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                    if (PB1_PORT)
                        Disp2String("\n\r[DEBUG] PB1 at reset: 1 (HIGH)\n\r");
                    else
                        Disp2String("\n\r[DEBUG] PB1 at reset: 0 (LOW)\n\r");
                    xSemaphoreGive(uart_sem);

                    oncePrinted = 1;
                }

               
                if (PB1_PORT == 0)     // button pressed
                {
                    // crude debounce: wait a bit, then check again
                    vTaskDelay(pdMS_TO_TICKS(50));

                    if (PB1_PORT == 0) // still pressed after 50 ms
                    {
                        xSemaphoreTake(uart_sem, portMAX_DELAY);
                        Disp2String("\n\r[WAITING] PB1 press detected, moving to TIME_ENTRY.\n\r");
                        xSemaphoreGive(uart_sem);

                        // wait for release so we don't immediately retrigger
                        while (PB1_PORT == 0)
                        {
                            vTaskDelay(pdMS_TO_TICKS(10));
                        }

                        waitingPromptShown = 0;   // so we reprint message next time we come back
                        currentState = STATE_TIME_ENTRY;
                    }
                }

                // Run this state quickly to feel responsive
                vTaskDelay(pdMS_TO_TICKS(10));
                break;
            }

            case STATE_TIME_ENTRY:
            {
                // Stop LED2 pulsing while entering time
                dutyTicks = 0;
                LED2_LAT  = 0;

                LED0_LAT = 0;
                LED1_LAT = 0;

                waitingPromptShown = 0; // so WAITING message runs again next time we enter

                xSemaphoreTake(uart_sem, portMAX_DELAY);
                Disp2String("\n\r[TIME ENTRY] Please enter time as MMSS (e.g., 0130 for 1min 30s), then press ENTER:\n\r> ");
                xSemaphoreGive(uart_sem);

                // Blocking receive; echoes characters as typed
                RecvUart(inputBuf, sizeof(inputBuf));
                inputBuf[sizeof(inputBuf)-1] = '\0';

                // Extract digits only
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
                    // invalid -> default 00:10
                    mm = 0;
                    ss = 10;
                }

                if (ss > 59) ss = 59;

                gMinutes = (uint16_t)mm;
                gSeconds = (uint16_t)ss;

                // Do NOT start countdown immediately.
                xSemaphoreTake(uart_sem, portMAX_DELAY);
                Disp2String("\n\r[TIME ENTRY] Time set.\n\r");
                Disp2String("[TIME ENTRY] Click PB2 and PB3 together to start.\n\r");
                Disp2String("[TIME ENTRY] Long press PB2+PB3 to reset and re-enter time.\n\r");
                xSemaphoreGive(uart_sem);

                // Wait here for PB2+PB3 short or long press
                {
                    uint16_t comboHoldTicks = 0;
                    uint8_t  comboActive    = 0;
                    const uint16_t COMBO_LONG_PRESS_MS    = 1000; // ~1s long press
                    const uint16_t COMBO_TICK_MS          = 20;
                    const uint16_t COMBO_LONG_PRESS_TICKS = (COMBO_LONG_PRESS_MS / COMBO_TICK_MS);

                    for (;;)
                    {
                        uint8_t p2 = PB2_PORT;  // assuming pull-up -> 1 = released, 0 = pressed
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
                                // both were pressed and now at least one released
                                if (comboHoldTicks >= COMBO_LONG_PRESS_TICKS)
                                {
                                    // Long press: reset time and re-enter
                                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                                    Disp2String("\n\r[TIME ENTRY] Long press PB2+PB3 detected. Resetting time.\n\r");
                                    xSemaphoreGive(uart_sem);

                                    gMinutes = 0;
                                    gSeconds = 0;
                                    // break: outer loop will re-run STATE_TIME_ENTRY and ask again
                                }
                                else if (comboHoldTicks > 0)
                                {
                                    // Short click: start countdown
                                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                                    Disp2String("\n\r[TIME ENTRY] Starting countdown.\n\r");
                                    xSemaphoreGive(uart_sem);

                                    countdownInitialised = 0;
                                    currentState = STATE_COUNTDOWN;
                                }

                                break;  // exit wait loop after action
                            }

                            // no active combo
                            comboActive    = 0;
                            comboHoldTicks = 0;
                        }

                        vTaskDelay(pdMS_TO_TICKS(COMBO_TICK_MS));
                    }
                }

                break;
            }


            case STATE_COUNTDOWN:
            {
                // Initialize once when we first enter
                if (!countdownInitialised)
                {
                    LED0_LAT = 0;
                    LED1_LAT = 1;  // start with LED1 on for 1 Hz blink
                    LED2_LAT = 0;  // LED2 brightness via dutyTicks + ADC

                    // reset pulsing state for LED2; now LED2 will be driven by ADC logic
                    pulseCounter = 0;
                    dutyStep     = 1;

                    xLastWakeTime = xTaskGetTickCount();
                    countdownTickCounter = 0;
                    countdownPaused      = 0;
                    pb3HoldTicks         = 0;
                    led2BlinkPhase       = 1;   // start LED2 "on" phase
                    led2BlinkMode        = 1;   // default: blink in countdown

                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                    Disp2String("\n\r[COUNTDOWN] Countdown started.\n\r");
                    Disp2String("[COUNTDOWN] Click PB3 to pause/resume. Long press PB3 to abort.\n\r");
                    Disp2String("[COUNTDOWN] Type 'i' to toggle extra info, 'b' to toggle LED2 blink/solid.\n\r");
                    xSemaphoreGive(uart_sem);

                    countdownInitialised = 1;
                }

                // Run this state every COUNTDOWN_TICK_MS (e.g., 100 ms)
                vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(COUNTDOWN_TICK_MS));

                // --- PB3: pause/resume (short click) and abort (long press) ---
                {
                    uint8_t pb3 = PB3_PORT;  // assuming pull-up: 1 = released, 0 = pressed

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
                                // Long press: abort to 0:00 and go to DONE
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
                                break; // leave COUNTDOWN state
                            }
                            else
                            {
                                // Short click: toggle pause/resume
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

                // --- Handle UART commands 'i' and 'b' (non-blocking) ---
                if (RXFlag)
                {
                    char c = received_char;
                    RXFlag = 0;      // clear flag so we see the next char

                    if (c == 'i')
                    {
                        // toggle extra info display mode
                        showExtraInfo ^= 1;
                    }
                    else if (c == 'b')
                    {
                        // toggle LED2 mode (blink vs solid)
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

                // --- ADC: read potentiometer and update LED2 brightness (always) ---
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

                // --- 1 second bookkeeping ---
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

                            // Blink LED1 at 1 Hz: toggle every second
                            LED1_LAT ^= 1;  // 1s on / 1s off

                            // LED2 blink phase: toggle once each second if in blink mode
                            if (led2BlinkMode)
                            {
                                led2BlinkPhase ^= 1;
                            }
                        }
                    }

                    // --- Print time (simple or extended) ---
                    if (!showExtraInfo)
                    {
                        // simple view: time only
                        PrintTimeUART(gMinutes, gSeconds);
                    }
                    else
                    {
                        // extended view: time + ADC + duty + mode
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

                    // Reached 0?
                    if (!countdownPaused && gMinutes == 0 && gSeconds == 0)
                    {
                        currentState     = STATE_DONE;
                        doneBlinkCount   = 0;
                        doneMessageShown = 0;
                    }
                }

                break;
            }


            case STATE_PAUSED:
                // Not used; pause handled inside STATE_COUNTDOWN with countdownPaused flag.
                vTaskDelay(pdMS_TO_TICKS(100));
                break;

            case STATE_DONE:
            {
                // After the timer has elapsed, the terminal should message that the countdown is done,
                // and LED0 and LED1 should blink rapidly in an alternating fashion.
                // LED2 should remain solidly on. After 5 seconds , return to WAITING.

                if (!doneMessageShown)
                {
                    xSemaphoreTake(uart_sem, portMAX_DELAY);
                    Disp2String("\n\r[DONE] Countdown complete! Timer reached 00:00.\n\r");
                    xSemaphoreGive(uart_sem);

                    doneMessageShown = 1;
                    doneBlinkCount = 0;

                    LED0_LAT = 1;
                    LED1_LAT = 0;

                    // LED2: solid, brightness from ADC (via PWM)
                    led2BlinkMode = 0;  // solid mode
                }

                // Alternate LED0 and LED1 quickly (100 ms period)
                LED0_LAT ^= 1;
                LED1_LAT = !LED0_LAT;
                
                // LED2 brightness still controlled by potentiometer (solid)
                {
                    uint16_t adcVal = do_ADC();
                    lastAdcVal = adcVal;
                    led2DutyFromADC = (uint32_t)adcVal * DUTY_MAX_TICKS / 1023u;
                    if (led2DutyFromADC > DUTY_MAX_TICKS)
                    {
                        led2DutyFromADC = DUTY_MAX_TICKS;
                    }

                    // In DONE, LED2 should be solid at this brightness
                    dutyTicks = led2DutyFromADC;
                }

                vTaskDelay(pdMS_TO_TICKS(100));
                doneBlinkCount++;

                // 100ms * 50 = 5s
                if (doneBlinkCount >= 50)
                {
                    // Turn LEDs off and go back to waiting
                    LED0_LAT = 0;
                    LED1_LAT = 0;
                    LED2_LAT = 0;

                    gMinutes = 0;
                    gSeconds = 0;
                    countdownInitialised = 0;

                    // reset pulsing variables for WAITING_ST (for LED2 pulsing)
                    dutyTicks   = 0;
                    dutyStep    = 1;
                    pulseCounter = 0;

                    currentState = WAITING_ST;
                    waitingPromptShown = 0;
                }

                break;
            }


            default:
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
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
    
    // PB1 (RA4) as input
    PB1_TRIS = 1;         // RA4 as input

    // RA4 pull-up
    CNPUAbits.CNPUA4 = 1;
    
    // PB2 and PB3
    PB2_TRIS = 1;
    PB3_TRIS = 1;
    
    // ensure PB2/PB3 pins are DIGITAL, not analog
    //ANSELBbits.ANSB8 = 0;   // PB2 on RB8 as digital input
    ANSELBbits.ANSB9 = 0;   // PB3 on RB9 as digital input

    // enable pull-ups on RB8 and RB9
    CNPUBbits.CNPUB8 = 1;   // RB8 pull-up
    CNPUBbits.CNPUB9 = 1;   // RB9 pull-up


    // Timer 2 for LED pulsing and PWM
    InitTimer2ForPWM();
    
}

int main(void) {
    
    prvHardwareSetup();

    uart_sem = xSemaphoreCreateMutex();
    
    // Higher priority than LED task 
    xTaskCreate( vMainTimerTask, "MainTimerTask", TASK_STACK_SIZE, NULL, 2, NULL );

    
    vTaskStartScheduler();
    
    for(;;);
}
