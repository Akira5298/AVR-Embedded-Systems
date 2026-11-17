#include <avr/io.h>
#include <util/delay.h>



const uint8_t segments[10] = {
   0xFC, // 0

   0x60, // 1
   0xDA, // 2
   0xF2, // 3
   0x66, // 4
   0xB6, // 5
   0xBE, // 6
   0xE0, // 7
   0xFE, // 8
   0xF6  // 9
};

int main(void) {
    // Set Port A as output
    
    DDRA = 0xFF;
    uint8_t count = 0;
    
    while (1) {
        PORTA = segments[count];
        
        // Wait for 1 second
        _delay_ms(1000);
        // Increment the count and wrap around to 0 after 9
        
        count = (count + 1) % 10;
    }
    
    return 0;
}


