#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import os
import pandas as pd
from matplotlib import pyplot as plt

#df = pd.read_csv('stats/ohlc_bitfinex_btcusd_21600_10_250_0.9_0.7.csv', index_col=False, sep='\t')

for file in os.listdir('log'):
    if '.csv' not in file or '10_100_0.9_0.7' not in file:
        continue
    df = pd.read_csv(os.path.join('log', file))
    plt.figure()
    plt.ylabel('Greška')
    plt.title(file)
    plt.plot(df.groupby('Generation').mean())
    plt.legend(['Train', 'Test'])
    plt.savefig('img/{}.pdf'.format(file.replace('_10_100_0.9_0.7.csv', '')))