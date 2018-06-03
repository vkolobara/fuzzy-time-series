# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from financial_time_series import *

set_name = 'SP500'

df = pd.read_csv("../../data/raw/{}.csv".format(set_name))

'''
df = df.rename(columns={
            "ClosePrice":"Close",
            "LowPrice":"Low",
            "OpenPrice":"Open",
            "HighPrice":"High"
        })
'''

#df = df[df['Close'] > 0.5]

test = stochastic_oscillator_k(df, 20)
test = relative_strength_index(test, 20)
test = macd(test)
test['pct_change'] = test['Close'].pct_change()
test['pred'] = test.shift(-1)['Open']

test = test[['pct_change', 'Volume', 'SOk', 'RSI_20', 'pred']].dropna()

split_ix = int(test.shape[0] * 0.7)

#test = test[['Close']].dropna()

test.iloc[:split_ix].to_csv('../../data/train/{}.csv'.format(set_name), index=False)
test.iloc[split_ix:].to_csv('../../data/test/{}.csv'.format(set_name), index=False)
