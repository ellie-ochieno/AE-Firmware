'''
#  ===================
#  SMART FARM PROJECT =============================
#  ===================
#  TEMPERATURE&HUMIDITY CONTROL--------------------
#  - SENSOR: DHT22
#  - PROTOCOL: MQTT
#  - Monitor/control parameters from:
#     -> Hardware module - collected readings
#     -> Platform - set controls
#  AMGAZA ELIMU - SF. V1.0|023
#  ------------------------------------------------
'''

#  Support libraries-------------------------------
import network                              # Native libs
import time
import random
from machine import Pin
import dht 
                                            # Third Party
from umqtt.robust import MQTTClient
                                            # Internal libs(customized)
import secrets                              # auths
import topics                               # client topics
import mod_connect                          # connections handler
#  ------------------------------------------------

#  Control parameters------------------------------
dht_sensor = dht.DHT22(Pin(3))              # dht dht_sensor instance dfn
dht_temp = dht_humid = 0                    # sensor parameters holder variables dfn
#  ------------------------------------------------

'''
    DHT sensor reading handler
    - For temperature
'''  
def get_dhttemp_reading():
    try:                                     # validate and take sensor readings
        time.sleep(2)
        dht_sensor.measure()                 # initialize sensor to reading mode
        dht_temp = round(dht_sensor.temperature(), 1)
        return dht_temp                      # return sensor value calibrated to 1d.p
    except OSError as e:
        print('Failed to read dht sensor.')
        print("\n")
           
'''
    DHT sensor reading handler
    - For humidity
'''
def get_dhthumid_reading():
    try:                                     # validate and take sensor readings
        time.sleep(2)
        dht_sensor.measure()                 # initialize sensor to reading mode
        dht_humid = round(dht_sensor.humidity(), 1)
        return dht_humid                     # return sensor value calibrated to 1d.p
    except OSError as e:
        print('Failed to read dht sensor.')

'''
    Module connection init handler
    - To both:
        - WiFi & mqtt broker
'''
def connections_init():
                                             # init wifi connection
    mod_connect.wifi_connect(secrets.Wifi_NAME, secrets.wifi_PASSWORD)
    print('Connecting to mqtt broker...\n')
    return mod_connect.mqtt_connect()        # init broker connection as return type

'''
    ----------------
    PUBLISH HANDLER ---------------------
    ----------------
    MQTT broker publish info handler
    - Publish msg(vaalue) to specified topic(topic)
'''
def publish(topic, value, client):
    client.publish(topic, value)              # publish topic
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
def mqtt_broker_res(topic, response):         # fetch and display subscribed topic &msg data
    print("\nReceived message on topic:", topic.decode())
    print("Message data:", response.decode(),"\n")          

'''
    MQTT broker topic subscription handler
    - Subscribes to passed-in topic
    - This initializes receiving data from the broker
'''
def subscribe(topic, client):
    client.subscribe(topic)                   # topic subscription
    # print("Successfully subscribed to topic: ", topic) # confirm msg

'''
    ---------------- END ----------------
'''

'''
    Invoke MQTT broker/client communication handlers
'''
client = connections_init()                   # wifi&broker connections
client.set_callback(mqtt_broker_res)          # callback response handler
                                              # topics subscription
subscribe(topics.topic_DHTTEMP, client)       
subscribe(topics.topic_DHTHUMID, client)      

'''
    Set delay btn broker iteration queries
'''
while True:
    dht_temp = get_dhttemp_reading()          # call dht temperature reader fn
    dht_humid = get_dhthumid_reading()        # call dht humidity reader fn
                                              # publish sensor readings to specified topics
    if dht_temp != None:
        publish(topics.topic_DHTTEMP, str(dht_temp), client)
    if dht_humid != None:
        publish(topics.topic_DHTHUMID, str(dht_humid), client)
    
    time.sleep(5)                              # publish after very 5s
    client.check_msg()                         # invokes check-msgs handler

