# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from financial_time_series import *
import sys,os

file_path = sys.argv[1]
set_name = os.path.splitext(os.path.basename(file_path))[0]

#df = pd.read_csv("../../data/raw/{}.csv".format(set_name), index_col=False).drop_duplicates('CloseTime')
df = pd.read_csv(file_path, index_col=False).drop_duplicates('CloseTime')

df.set_index('CloseTime')

df = df.rename(columns={
            "ClosePrice":"Close",
            "LowPrice":"Low",
            "OpenPrice":"Open",
            "HighPrice":"High"
        })


df = df[df['Close'] > 1e-7]

t = int(os.path.splitext(set_name.split('_')[-1])[0])

period = 60

if t == 180:
    period = 20
elif t == 300:
    period = 12
elif t == 21600:
    period = 4
elif t == 86400:
    period = 7

test = stochastic_oscillator_k(df, period)
test = relative_strength_index(test, period)
test = macd(test)
test['pct_change'] = test['Close'].pct_change()
test['pred'] = test.shift(-1)['Open']

test = test[['pct_change', 'Volume', 'SOk', 'RSI_{}'.format(period), 'pred']].dropna()

split_ix = int(test.shape[0] * 0.7)

#test = test[['Close']].dropna()

test.iloc[:split_ix].to_csv('../../data/train/{}.csv'.format(set_name), index=False)
test.iloc[split_ix:].to_csv('../../data/test/{}.csv'.format(set_name), index=False)
