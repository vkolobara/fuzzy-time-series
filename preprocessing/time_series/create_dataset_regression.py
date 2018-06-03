# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from time_series import *

set_name = 'pigs'

df = pd.read_csv("../../data/raw/{}.csv".format(set_name))

df['pred'] = df['Value'].shift(-1)
df = df[['Value', 'pred']].dropna()

split_ix = int(df.shape[0] * 0.7)

df.iloc[:split_ix].to_csv('../../data/train/{}.csv'.format(set_name), index=False)
df.iloc[split_ix:].to_csv('../../data/test/{}.csv'.format(set_name), index=False)
