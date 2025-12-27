/*
    Pseudocode to demonstrate switch debouncing in software
    Assume the switch datasheet specs a 12.5ms bounce.
    If there are 10 button presses/second i.e 0.1s for 1 button press cycle(HIGH + LOW)
    so a button press cycle (low + high) takes 100ms total. 
    
    We want to debounce the switch so since we know it takes 50ms for the low signal for a single button press,
    this implementation takes a conservative approach to take 5 samples. the number of samples can be changed. 
    The timer interrupt is configured for every 10ms(since bounce duration for the button press is 12.5ms sample
    at a freq higher than that so that we don't miss the bounce and keep updating button states correctly)
*/

#define PRESSED         true
#define RELEASED        false
#define NUM_SAMPLES      5

volatile bool current_stable_state = RELEASED; //use this variable in the main application logic

//Interrupt Service Routine called every 10ms (can be configured on rising or falling edge of button press)
void timer_interrupt_handler()
{
    static int counter = 0;
    static bool last_button_state = RELEASED;
    

    //read raw button state from the I/O pin
    bool raw_state = read_button_state();

    if(raw_state == last_button_state)
    {
        counter++;

        if(counter >= NUM_SAMPLES)
        {   
            //we have collected enough samples to update
            current_stable_state = raw_state;
            counter = NUM_SAMPLES;
        }
    }
    else
    {
        //reset the counter and update the last button state
        counter = 0;
        last_button_state = raw_state;
    }

}