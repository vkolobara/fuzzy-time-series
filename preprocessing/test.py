# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from fin_time_series.financial_time_series import *

df = pd.read_csv("../data/SP500.csv")
df = df.set_index("Date")

test = moving_average(df, 7)
test = bollinger_bands(test, 7)
test = stochastic_oscillator_k(test)
test = stochastic_oscillator_d(test, 7)
test = macd(test)
test['pred'] = test.shift(-1)['Close']

test = test[['Close', 'Volume', 'MA_7', 'BollingerB1_7', 'BollingerB2_7', 'SOk', 'SOd_7', 'MACD_15_26', 'MACDSign_15_26', 'MACDDiff_15_26', 'pred']].dropna()

test = test[['Close']].dropna()

test.to_csv('../data/SP500_prep_price.csv', index=False)
