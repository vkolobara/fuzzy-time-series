#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import os
import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('stats/ohlc_bitfinex_btcusd_21600_10_250_0.9_0.7.csv', index_col=False, sep='\t')

data = []

curr_max = 0
best = ''

for file in os.listdir('stats'):
    data.append(pd.read_csv(os.path.join('stats', file), index_col=False, sep='\t')['fit_max'])
    if data[-1].max() > curr_max:
        best = file
        curr_max = data[-1].max()
    
plt.boxplot(data)
plt.show()