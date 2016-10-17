#ifndef SERVO_H_
#define SERVO_H_

#define PWM_GO		3686
#define PWM_REVERSE	1843
#define PWM_NEW		2000
#define PWM_START	2765
#define PWM_STOP		2764

#define PWM_LEFT			1843
#define PWM_RIGHT			3686
#define PWM_STRAIGHT		2764
#define PWM_SLIGHT_LEFT	2664
#define PWM_SLIGHT_RIGHT	2864

void pwm_set(uint16_t x) {
  OCR1B = x;
}
void pwm_steering_set(uint16_t y) {
  OCR1A = y;
}
void pwm_init() {
  // setup Timer1 for Fast PWM mode, 16-bit
  // COM1B1 -- for non-inverting output
  // WGM13, WGM12, WGM11, WGM10 -- for Fast PWM with OCR1A as TOP value
  // CS11 -- for CLK/8 prescaling
  
  ICR1 = 36864;	// sets PWM to repeat pulse every 20.0ms
  pwm_set(PWM_START);
  pwm_steering_set(PWM_STRAIGHT);  
  TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
  TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
  
  // each count is 8/14745600 = 0.5425us.
  // so 1.0ms = 1843.2
  //    1.5ms = 2764.8
  //    2.0ms = 3686.4
  //   20.0ms = 36864
}

#endif//SERVO_H_
