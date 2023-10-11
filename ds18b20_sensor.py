'''
#  ===================
#  SMART FARM PROJECT ============================
#  ===================
#  SOIL TEMPERATURE SENSOR (DS18B20) CONTROL-------------------
#  Control/monitor soil temperature from:
#     -> Platform - set controls
#  AMGAZA ELIMU - SF. V1.0 / 023
#  ------------------------------------------------
'''
#  Import support libraries -----------------------
import onewire
import ds18x20
import time
import machine 
from machine import Pin
                                        # Third Party
from umqtt.robust import MQTTClient
                                        # Internal libs(customized)
import secrets                          # auths
import topics                           # client topics
import mod_connect                      # connections handler
#  ------------------------------------------------

#  Control parameters------------------------------
                                        # Instantiate sensor object
ds18b20_sensor = ds18x20.DS18X20(onewire.OneWire(Pin(21)))

ds_18 = ds18b20_sensor.scan()           # scan for the DS18B20 sensor on the onewire bus
 
'''
    DS18B20 sensor reading handler
    - For soil temperature
'''
def get_soil_temperature():
                                        # Read the soil temperature values from the sensor
    ds18b20_sensor.convert_temp()       
    soil_temperature = ds18b20_sensor.read_temp(ds_18[0])
                                        # Print the soil temperature value on the serial monitor
    print("Soil Temperature: %f °C\n" % soil_temperature) 

    return soil_temperature


'''
    Module connection init handler
    - To both:
        - WiFi & mqtt broker
'''
def connections_init():
                                        # init wifi connection
    mod_connect.wifi_connect(secrets.Wifi_NAME, secrets.wifi_PASSWORD)
    print('Connecting to mqtt broker...\n')
    return mod_connect.mqtt_connect()   # init broker connection as return type

'''
    ----------------
    PUBLISH HANDLER ---------------------
    ----------------
    MQTT broker publish info handler
    - Publish msg(vaalue) to specified topic(topic)
'''
def publish(topic, value, client):
    client.publish(topic, value)        # publish topic
    # print("Successfully published to topic: ", topic)  # confirm msg confirm msg

'''
    ---------------- END ----------------
'''

'''
    ---------------------
    SUBSCRIPTION HANDLER ----------------
    ---------------------
    MQTT broker callback response handler
    - Receive published info to subscribed topic:
        - Topic & message/data
'''
def mqtt_broker_res(topic, response):    # fetch and display subscribed topic &msg data
    print("\nReceived message on topic:", topic.decode())
    print("Message data:", response.decode(),"\n")          

'''
    MQTT broker topic subscription handler
    - Subscribes to passed-in topic
    - This initializes receiving data from the broker
'''
def subscribe(topic, client):
    client.subscribe(topic)               # topic subscription
    # print("Successfully subscribed to topic: ", topic) # confirm msg

'''
    ---------------- END ----------------
'''

'''
    Invoke MQTT broker/client communication handlers
'''
client = connections_init()               # wifi&broker connections
client.set_callback(mqtt_broker_res)      # callback response handler
                                          # topics subscription
subscribe(topics.topic_SOILTEMP, client)       

'''
    Set delay btn broker iteration queries
'''
while True:
    soil_temp = get_soil_temperature()    # call soil temperature reader fn
    
                                          # publish sensor readings to specified topic
    if soil_temp != None:
        publish(topics.topic_SOILTEMP, str(soil_temp), client)
    
    time.sleep(5)                         # publish after very 5s
    client.check_msg()                    # invokes check-msgs handler

