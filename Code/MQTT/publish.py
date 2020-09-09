# MQTT Publish Demo
# Publish two messages, to two different topics
import serial
ser = serial.Serial('/dev/ttyACM0',9600)

import paho.mqtt.publish as publish
MQTT_SERVER = "192.168.137.57"
publish.single("test_channel", "Helloggg", hostname=MQTT_SERVER)
print("Done")

while(True):
    s=ser.readline()
    if s[0] =='1':
        publish.single("notification",s[1:] , hostname=MQTT_SERVER)
    if s[0] =='2':
        publish.single("sensor_value",s[1:], hostname=MQTT_SERVER)
    if s[0] == '3':
        publish.single("debug",s[1:] , hostname=MQTT_SERVER)
    if s[0] == '4':
        publish.single("plant_health",s[1:], hostname=MQTT_SERVER)