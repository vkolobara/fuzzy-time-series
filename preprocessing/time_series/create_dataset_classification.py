# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd

set_name = 'iris'

df = pd.read_csv("../../data/raw/{}.csv".format(set_name)).sample(frac=1)

split_ix = int(df.shape[0] * 0.7)

df.iloc[:split_ix].to_csv('../../data/train/{}.csv'.format(set_name), index=False)
df.iloc[split_ix:].to_csv('../../data/test/{}.csv'.format(set_name), index=False)
