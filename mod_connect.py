'''
#  ===================
#  CONNECTIONS HADLER =============================
#  ===================
#  WiFi + MQTT
#  AMGAZA ELIMU - SF. V1.0|023
#  ------------------------------------------------
'''

#  Support libraries-------------------------------
import network                          # Native libs
import time
import random
                                        # Third Party
from umqtt.robust import MQTTClient
                                        # Internal libs(customized)
import secrets                          # auths
#  ------------------------------------------------

'''
    MQTT broker connection handler
    - Uses broker auth parameters
'''
def mqtt_connect():
                                          # mqtt client instance dfn
    client = MQTTClient(
        client_id=b"ae_sf_hivemq",        # mqtt broker client ID (can be anything)
        server=secrets.MQTT_HOSTNAME,     # mqtt broker hostname(cluster url)
        port=0,                           # mqtt connection port(set to "0" by default)
        user=secrets.MQTT_USERNAME,       # mqtt broker web client connection username
        password=secrets.MQTT_PASSWORD,   # mqtt broker web client connection password
        keepalive=7200,                   # mqtt client-broker timeout controller
        ssl=True,                         # set security auth flag to secure
        ssl_params={"server_hostname": secrets.MQTT_HOSTNAME}
    )
    client.connect()                      # mqtt client connection init 
    return client                         # returns mqtt client instance

'''
    WiFi connection handler
    - Receives two primary args(wifi ssid_name and password)
'''
def wifi_connect(ssid, password):
    wlan = network.WLAN(network.STA_IF)    # wifi connection instance init 
    wlan.active(True)                      # connection state dfn
    wlan.connect(ssid, password)           # connection init
    
    max_wait = 10                          # connection timeout
    wait_count = 0
    
    while max_wait > 0:                    # connection waiting loop
      if wlan.status() < 0 or wlan.status() >= 3:
        break
      max_wait -= 1
      print('Waiting for connection...')
      time.sleep(1)
                                            # connection error exception handler
    if wlan.status() != 3:
       print(wlan.status())
       raise RuntimeError('network connection failed')
    else:                                   # successful connection confirm
      print("Connecting to wifi")
      while(wait_count < 3):
          print(".", end="")
          wait_count +=1
          time.sleep(1)
      print("\n")
      print("Connected!")
      time.sleep(1)
      print("Status: ", wlan.status(),"\n") # print connection waiting status
      time.sleep(1)
      status = wlan.ifconfig()              # initialize connection status to wifi parameters
      