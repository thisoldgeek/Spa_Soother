# Adapted from Bob Rathbone's Simple Raspberry PI Internet radio using 
# four buttons
import RPi.GPIO as GPIO
import os
import atexit
import time
from time import sleep
import sys
import serial

ser = serial.Serial(port='/dev/ttyAMA0', baudrate = 9600, timeout=1)

startup = time.time()
ShutdownTime = 1200   # Pi will send a signal to Rainbowduino after 20 minutes

# Switch definitions, 
# using BOARD description
BUTTON1 = 11   #BCM 17, GPIO0 (zero)
BUTTON2 = 13   #BCM 27, GPIO2
BUTTON3 = 15   #BCM 22, GPIO3
BUTTON4 = 16   #BCM 23, GPIO4 

# Register exit routine
def finish():
     print("Sound Board Stopped")
     ser.write("Shutdown") # Signal to Arduino to stop power
     sleep(0.5)
     ser.close()
     print("Shutting Down Now!")
     GPIO.cleanup()
     os.system("sudo shutdown -h now")

atexit.register(finish)

# Execute system command sub-routine
def exec_command(cmd):
     result = ""
     p = os.popen(cmd)
     for line in p.readline().split('\n'):
          result = result + line
     return result

### Main routine ###
if __name__ == "__main__":
     GPIO.setmode(GPIO.BOARD)
     GPIO.setwarnings(False)  
     GPIO.setup(BUTTON1,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
     GPIO.setup(BUTTON2,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
     GPIO.setup(BUTTON3,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
     GPIO.setup(BUTTON4,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
     #exec_command("service mpd start")
     exec_command("mpc clear")
     exec_command("mpc load mylist")
     exec_command("mpc play 3")  #start with favorite
     exec_command("mpc volume 90")

     while True:
          check = time.time()
          elapsed = (int(round(check-startup)))
          if (elapsed > ShutdownTime):
             sys.exit()
          if GPIO.input(BUTTON1): 
               exec_command("mpc play 1")
          if GPIO.input(BUTTON2):
               exec_command("mpc play 2")
          if GPIO.input(BUTTON3): 
               exec_command("mpc play 3")
          if GPIO.input(BUTTON4): 
               exec_command("mpc play 4")
          sleep(0.1)


GPIO.cleanup()
