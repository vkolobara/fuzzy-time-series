# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from financial_time_series import *
import random

df = pd.read_csv("../data/ohlc_bitfinex_iotusd_60.csv").drop_duplicates(['CloseTime'])
df = df.set_index("CloseTime")


df = df.rename(columns={
            "ClosePrice":"Close",
            "LowPrice":"Low",
            "OpenPrice":"Open",
            "HighPrice":"High"
        })

balance = 100.0
bought = 0

vals = df['Close'].values

for i in range(len(vals)):
    rnd = random.uniform(-1, 1)

    if rnd < -0.9 and bought > 0:
        bought -= 1
        balance += vals[i]
        balance -= 0.002
    if rnd > 0.9 and vals[i] < balance:
        bought += 1
        balance -= vals[i]
        balance -= 0.002

print(balance/100.0)

