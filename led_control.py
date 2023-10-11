#  ===================
#  SMART FARM PROJECT ============================
#  ===================
#  ONBOARD LED CONTROL-------------------
#  Control/monitor LED status from:
#     -> Platform - set controls
#  AMGAZA ELIMU - SF. V1.0
#  ------------------------------------------------

#  Support libraries-------------------------------
                                        # Native libs
import time
import random
import network                          
import ubinascii
from machine import Pin
                                        # Third Party
from umqtt.robust import MQTTClient
                                        # Internal libs(customized)
import topics                           # client topics
import secrets                          # auths
import mod_connect                      # connections handler
#  ------------------------------------------------

#  Control parameters------------------------------
                                        # Onboard LED instance state dfn
led = machine.Pin("LED",machine.Pin.OUT)

                                        # Publish MQTT messages after every set timeout
last_publish = time.time()
publish_interval = 5

#  ------------------------------------------------
    

'''
    Module connection init handler
    - To both:
        - WiFi & mqtt broker
'''
def connections_init():
                                        # init wifi connection
    mod_connect.wifi_connect(secrets.Wifi_NAME, secrets.wifi_PASSWORD)
    print('Connecting to mqtt broker...\n')
    time.sleep(2)
    print("Waiting for LED state control signal")
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

'''
    ---------------------
    SUBSCRIPTION HANDLER ----------------
    ---------------------
    MQTT broker callback response handler
    - Receive published info to subscribed topic:
        - Topic & message/data
'''
def mqtt_broker_res(topic, response):    # fetch and display subscribed topic &msg data
    print("\nCallback topic:", topic.decode())
    print("Signal state:", response.decode(),"\n")
                                         # LED states control
    if response.decode() == "ON" or response.decode() == "on":
        led.value(1)                     # turn LED "ON"
    elif response.decode() == "OFF" or response.decode() == "off":
        led.value(0)                     # turn LED "OFF"
    else:                                # error for undefined signal state
        print("Error!\nUndefined LED control state(Please use \"ON/OFF\").")    

'''
    MQTT broker topic subscription handler
    - Subscribes to passed-in topic
    - This initializes receiving data from the broker
'''
def subscribe(topic, client):
    client.subscribe(topic)              # topic subscription


'''
    ---------------- END ----------------
'''

'''
    Invoke MQTT broker/client communication handlers
'''
client = connections_init()              # wifi&broker connections
client.set_callback(mqtt_broker_res)     # callback response handler
                                             
subscribe(topics.topic_LED, client)      # topics subscription

'''
    Set delay btn broker iteration queries
'''
while True:
    
    time.sleep(5)                        # publish after very 5s
    client.check_msg()                   # invokes check-msgs handler


