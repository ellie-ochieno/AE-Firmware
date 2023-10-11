'''
#  ===================
#  SMART FARM PROJECT ============================
#  ===================
#  TDS SENSOR CONTROL-------------------
#  Control/monitor TDS from:
#     -> Platform - set controls
#  AMGAZA ELIMU - SF. V1.0 / 023
#  ------------------------------------------------
'''

#  Import support libraries -----------------------
import time
import machine
from machine import Pin, ADC
                                        # Third Party
from umqtt.robust import MQTTClient
                                        # Internal libs(customized)
import secrets                          # auths
import topics                           # client topics
import mod_connect                      # connections handler
#  ------------------------------------------------

#  Control parameters------------------------------

                                        # Instantiate sensor Pins
tds_adc_pin = machine.ADC(28)

'''
    Sensor data reading handler
    - For TDS sensor
'''
def get_tds_readings():
    tds_calibration_factor = 800
    tds_analog_value = tds_adc_pin.read_u16()    # Read analog value
    voltage = tds_analog_value *(3.3/65535)      # Convert analog tds value to voltage
    tds_value = voltage * tds_calibration_factor # Calculate TDS value in ppm
    print("TDS value: %f ppm" % tds_value)       # Print TDS values on the serial monitor
    
    return tds_value


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
subscribe(topics.topic_SENSORTDS, client)       

'''
    Set delay btn broker iteration queries
'''
while True:
    tds_value = get_tds_readings()        # call tds reader fn
    
                                          # publish sensor readings to specified topic
    if tds_value != None:
        publish(topics.topic_SENSORTDS, str(tds_value), client)
    
    time.sleep(5)                         # publish after very 5s
    client.check_msg()                    # invokes check-msgs handler


