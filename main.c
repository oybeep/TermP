#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"

// =============================
//     SERVO INITIALIZATION
// =============================
void servo_init(void){
	DDRB |= (1 << PB5);   // OC1A = PB5

	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // prescaler 8

	ICR1 = 40000; // 20ms
}

// =============================
// SG90: 700~2400us 펄스
// =============================
void set_servo_angle(uint8_t angle){
	if(angle > 180) angle = 180;

	OCR1A = 700 + (uint32_t)angle * 1700UL / 180;
}

// =============================
//         MAIN
// =============================
int main(void){
	LCD_Init();
	LCD_Clear();

	servo_init();

	LCD_Pos(0,0);
	LCD_Str("Servo Angle:");

	char buf[8];
	uint8_t angle = 0;

	while(1){

		// 0 → 180
		for(angle = 0; angle <= 180; angle += 5){
			set_servo_angle(angle);

			LCD_Pos(0,1);   // 두 번째 줄
			itoa(angle, buf, 10);
			LCD_Str("     "); // 이전 값 지우기
			LCD_Pos(0,1);
			LCD_Str(buf);
			LCD_Str(" deg");

			_delay_ms(10);
		}

		// 180 → 0
		for(angle = 180; angle >= 5; angle -= 5){
			set_servo_angle(angle);

			LCD_Pos(0,1);
			itoa(angle, buf, 10);
			LCD_Str("     ");
			LCD_Pos(0,1);
			LCD_Str(buf);
			LCD_Str(" deg");

			_delay_ms(10);
		}
	}
}

