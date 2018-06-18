#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import pandas as pd
from matplotlib import pyplot as plt

df = pd.read_csv('log/ohlc_bitfinex_iotusd_86400_10_100_0.9_0.7.csv', index_col=False)

df_train = pd.read_csv('data/train/ohlc_bitfinex_iotusd_86400.csv')
df_test = pd.read_csv('data/test/ohlc_bitfinex_iotusd_86400.csv')

x = df.groupby('Generation').mean()

df_train['pred'].plot()
plt.axvline(len(df_train) - len(df_test), color='r')
plt.figure()
x.plot()