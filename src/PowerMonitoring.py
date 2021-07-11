#!/usr/bin/python3

import os
from requests import api
import thingspeak
import time
import requests
import json
import logging
import simplepush
import thingspeak
 
ts_channel_id = 1435107
ts_api_key = 'I7IVZRKLZP33XAQ0'
sp_keys = ['kJKkau',    # Georg
           't9zNDd'     # Papa
          ]
stPower = True
alive_counter_timeout = 180         # s
cycle_time = 60                     # s
 
if __name__ == "__main__":
    logName = os.path.join('PowerMonitoring.log')
    logging.basicConfig(filename=logName, filemode='w', level=logging.INFO, format='%(asctime)s: %(levelname)s: %(message)s')
    channel = thingspeak.Channel(id=ts_channel_id, api_key=ts_api_key)
    starttime = time.time()
    try:
        while True:
            try:
                last_data_age = channel.get_last_data_age(1)
                last_data_age = json.loads(last_data_age)
                t = int(last_data_age['last_data_age'])
                logging.info('t = {t}'.format(t=t))
                if t > alive_counter_timeout and stPower:
                    stPower = False
                    last_entry = channel.get_field_last(1, options={'timezone': 'Europe/Vienna'})
                    last_entry = json.loads(last_entry)
                    logging.info('Power failure: {}'.format(last_entry["created_at"]))
                    for key in sp_keys:
                        simplepush.send(key, 'Stromausfall', last_entry["created_at"], None)
                elif t <= alive_counter_timeout and not stPower:
                    stPower = True
                    last_entry = channel.get_field_last(1, options={'timezone': 'Europe/Vienna'})
                    last_entry = json.loads(last_entry)
                    logging.info('Power restored: {}'.format(last_entry["created_at"]))
                    for key in sp_keys:
                        simplepush.send(sp_keys, 'Stromversorgung OK', last_entry["created_at"], None)
            except requests.exceptions.HTTPError as e:
                print(e) 
                logging.error(e)
            except Exception as e:
                print(e)
                logging.exception(e)
            # delay; avoid drift
            time.sleep(float(cycle_time) - ((time.time() - starttime) % float(cycle_time)))
    except KeyboardInterrupt:
        print('end')
        logging.error("End")
