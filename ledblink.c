#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t pattern_type = 0;
volatile uint8_t button_pressed = 0;

void basic_pattern(uint8_t led) {
    uint8_t portd_mask = (led & 0xC0) >> 6;  
    uint8_t portb_mask = led & 0x3F;         

    
    PORTD = (PORTD & 0x3F) | (portd_mask << 6);
    PORTB = (PORTB & 0xC0) | portb_mask;
    _delay_ms(500);
}

void pat1() {
    basic_pattern(0xAA);
    if (button_pressed) return;
    basic_pattern(0x55);
    if (button_pressed) return;
}

void pat2() {
    for (int i = 0; i < 8; i++) 
        basic_pattern(1 << i);
        if (button_pressed) return;
    }
}

void pat3() {
    for (int i = 0; i < 4; i++) {
        basic_pattern((1 << i) | (1 << (7 - i)));
        if (button_pressed) return;
    }
}

void pat4() {
    for (int i = 0; i < 4; i++) {
        basic_pattern((1 << i) | (1 << (7 - i)));
        if (button_pressed) return;
    }
    for (int i = 3; i >= 0; i--) {
        basic_pattern((1 << i) | (1 << (7 - i)));
        if (button_pressed) return;
    }
}

void pat5() {
    for (int i = 0; i < 8; i++) {
        basic_pattern(1 << i);
        if (button_pressed) return;
    }
    for (int i = 7; i >= 0; i--) {
        basic_pattern(1 << i);
        if (button_pressed) return;
    }
}

void pat6() {
    uint8_t pt = 0x00;
    for (int i = 0; i < 8; i++) {
        pt |= (1 << i);
        basic_pattern(pt);
        if (button_pressed) return;
    }
    for (int i = 0; i < 8; i++) {
        pt &= ~(1 << i);
        basic_pattern(pt);
        if (button_pressed) return;
    }
}

void pat7() {
    uint8_t pt = 0x00;
    for (int i = 0; i < 8; i++) {
        pt |= (1 << i);
        basic_pattern(pt);
        if (button_pressed) return;
    }
    for (int i = 7; i >= 0; i--) {
        pt &= ~(1 << i);
        basic_pattern(pt);
        if (button_pressed) return;
    }
}

int main() {
    DDRD |= (1 << PD6) | (1 << PD7);
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2); 

    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    sei();

    while (1) {
        button_pressed = 0;
        patternCases();
    }
}

ISR(INT0_vect) {
    pattern_type++;
    if (pattern_type > 6) {
        pattern_type = 0;
    }
    button_pressed = 1;
}

void patternCases() {
    switch (pattern_type) {
        case 0: pat1(); break;
        case 1: pat2(); break;
        case 2: pat3(); break;
        case 3: pat4(); break;
        case 4: pat5(); break;
        case 5: pat6(); break;
        case 6: pat7(); break;
    }
}
