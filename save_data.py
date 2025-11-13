import serial
import time

arduino = serial.Serial('COM6', 9600) # порт, которым пользуется ардуинка!
time.sleep(2)

file = open('data.txt', 'w') # лежит в той же папке, что и этот код

try:
    while True:
        data = arduino.readline().decode().strip()
        print(data)
        file.write(data + '\n')
        file.flush()
except:
    file.close()
    arduino.close()