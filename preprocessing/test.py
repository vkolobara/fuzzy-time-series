# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from fin_time_series.financial_time_series import *

df = pd.read_csv("../data/ohlc_bitfinex_btcusd_21600.csv").drop_duplicates(['CloseTime'])
df = df.set_index("CloseTime")


df = df.rename(columns={
            "ClosePrice":"Close",
            "LowPrice":"Low",
            "OpenPrice":"Open",
            "HighPrice":"High"
        })

df = df[df['Close'] > 0.5]

test = stochastic_oscillator_k(df, 60)
test = relative_strength_index(test, 60)
test = macd(test)
test['pct_change'] = test['Close'].pct_change()
test['pred'] = test.shift(-1)['Open']

test = test[['pct_change', 'Volume', 'SOk', 'RSI_60', 'pred']].dropna()

split_ix = int(test.shape[0] * 0.7)

#test = test[['Close']].dropna()

test.iloc[:split_ix].to_csv('../data/btcusd_6h_prep.csv', index=False)
test.iloc[split_ix:].to_csv('../data/btcusd_6h_prep_test.csv', index=False)
