#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('log/ohlc_bitfinex_btcusd_21600_10_100_0.2_0.7.csv', index_col=False)

df_train = pd.read_csv('data/train/ohlc_bitfinex_btcusd_21600.csv')
df_test = pd.read_csv('data/test/ohlc_bitfinex_btcusd_21600.csv')

x = df.groupby('Generation').mean()

pd.concat([df_train['pred'], df_test['pred']], ignore_index=True).plot()
plt.axvline(len(df_train), color='r')
plt.figure()
x.plot()