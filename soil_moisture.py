'''
#  ===================
#  SMART FARM PROJECT ============================
#  ===================
#  SOIL MOISTURE SENSOR CONTROL-------------------
#  Control/monitor soil moisture from:
#     -> Platform - set controls
#  AMGAZA ELIMU - SF. V1.0 / 023
#  ------------------------------------------------
'''

#  Import support libraries -----------------------
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

                                        # Instantiate sensor Pins
moisture_adc_pin = machine.ADC(26)
moisture_digital_pin = Pin(20, Pin.IN)

'''
    Sensor data reading handler
    - For soil noisture(FC-28)
'''
def get_soil_moisture():
                                        # Read digital data
    digital_moisture_value = moisture_digital_pin.value()
                                        # Read analog data
    analog_moisture_value = moisture_adc_pin.read_u16()
                                        # compute data readings in percent
    soil_moisture_percentage = ((65535-analog_moisture_value) /65535) * 100 
                                                                           
                                        # print out recorded soil moisture values(formated to 2d.p)
    print("Digital soil moisture value: %.2f" % round(digital_moisture_value, 2))
    print("Analog soil moisture value: %.2f" % round(analog_moisture_value, 2))
    print("Soil Moisture: %.2f %%\n" % round(soil_moisture_percentage, 2))                 
    
    return soil_moisture_percentage


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
subscribe(topics.topic_SOILMOISTURE, client)       

'''
    Set delay btn broker iteration queries
'''
while True:
    soil_moisture = get_soil_moisture()   # call soil temperature reader fn
    
                                          # publish sensor readings to specified topic
    if soil_moisture != None:
        publish(topics.topic_SOILMOISTURE, str(soil_moisture), client)
    
    time.sleep(5)                         # publish after very 5s
    client.check_msg()                    # invokes check-msgs handler


