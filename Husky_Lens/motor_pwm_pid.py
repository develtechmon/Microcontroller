from machine import Pin
from machine import PWM
import utime

class motor_pid:
    MAX_DUTY_CYCLE = 65535
    MIN_DUTY_CYCLE = 0
    def __init__(self, motor_pins, frequency=20000):
        self.left_motor_pin1 = PWM(Pin(motor_pins[0], mode=Pin.OUT))
        self.left_motor_pin2 = PWM(Pin(motor_pins[1], mode=Pin.OUT))
        self.right_motor_pin1 = PWM(Pin(motor_pins[2], mode=Pin.OUT))
        self.right_motor_pin2 = PWM(Pin(motor_pins[3], mode=Pin.OUT))
        # set PWM frequency
        self.left_motor_pin1.freq(frequency)
        self.left_motor_pin2.freq(frequency)
        self.right_motor_pin1.freq(frequency)
        self.right_motor_pin2.freq(frequency)
        
        self.current_speed = motor_pid.MAX_DUTY_CYCLE
    
    ''' Map duty cycle values from 0-100 to duty cycle 40000-65535 '''
    def __map_range(self, x, in_min, in_max, out_min, out_max):
      return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min
        
    ''' new_speed is a value from 0% - 100% '''
    def change_speed(self, new_speed):
        new_duty_cycle = self.__map_range(new_speed, 0, 100, 40000, 65535)
        return new_duty_cycle
        #self.current_speed = new_duty_cycle
    
    def test(self,speed,turn):
        self.mySpeed = self.change_speed(speed)
        self.myTurn  = self.change_speed(turn)
        
        self.leftSpeed = self.mySpeed - self.myTurn
        self.rightSpeed = self.mySpeed + self.myTurn
                
        if self.leftSpeed > 65535 : self.leftSpeed = 65535
        elif self.leftSpeed <-65535: self.leftSpeed = -65535
        if self.rightSpeed > 65535 : self.rightSpeed = 65535
        elif self.rightSpeed <-65535: self.rightSpeed = -65535
        
        print(self.mySpeed, self.myTurn, self.leftSpeed, self.rightSpeed)

        if self.leftSpeed > 0 :
            print("left 1")
            self.left_motor_pin1.duty_u16(self.mySpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(motor_pid.MAX_DUTY_CYCLE)
            self.right_motor_pin2.duty_u16(motor_pid.MAX_DUTY_CYCLE)
        
        else:
            print("else 1")
            self.left_motor_pin1.duty_u16(self.mySpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.current_speed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)


        if self.rightSpeed > 0 :
            print("right 1")
            self.left_motor_pin1.duty_u16(motor_pid.MAX_DUTY_CYCLE)
            self.left_motor_pin2.duty_u16(motor_pid.MAX_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.rightSpeed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
        else:
            print("else 2")
            self.left_motor_pin1.duty_u16(self.mySpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.current_speed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
    
        
    def move(self, speed, turn, t=0):
        
        self.mySpeed = self.change_speed(speed)
        self.myTurn  = self.change_speed(turn)
        print(self.mySpeed, self.myTurn, self.leftSpeed, self.rightSpeed)
        
        self.leftSpeed = self.mySpeed - self.myTurn
        self.rightSpeed = self.mySpeed + self.myTurn
        

        if self.leftSpeed > 65535 : self.leftSpeed = 65535
        elif self.leftSpeed <-65535: self.leftSpeed = -65535
        if self.rightSpeed > 65535 : self.rightSpeed = 65535
        elif self.rightSpeed <-65535: self.rightSpeed = -65535
        
        print(self.mySpeed, self.myTurn, self.leftSpeed, self.rightSpeed)

        if self.leftSpeed > 0 :
            print("left 1")
            self.left_motor_pin1.duty_u16(self.leftSpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(motor_pid.MAX_DUTY_CYCLE)
            self.right_motor_pin2.duty_u16(motor_pid.MAX_DUTY_CYCLE)

        else:
            print("else 1")
            self.left_motor_pin1.duty_u16(self.mySpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.current_speed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)


        if self.rightSpeed > 0 :
            print("right 1")
            self.left_motor_pin1.duty_u16(motor_pid.MAX_DUTY_CYCLE)
            self.left_motor_pin2.duty_u16(motor_pid.MAX_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.rightSpeed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
        else:
            print("else 2")
            self.left_motor_pin1.duty_u16(self.mySpeed)
            self.left_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
            self.right_motor_pin1.duty_u16(self.current_speed)
            self.right_motor_pin2.duty_u16(motor_pid.MIN_DUTY_CYCLE)
        
        
if __name__ == '__main__':
    while True:
        motor_pins = [10,11,9,8]

        init = motor_pid(motor_pins, 20000)
        init.test(0,-60)
 

