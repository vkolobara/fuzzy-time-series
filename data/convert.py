# -*- coding: utf-8 -*-
"""
Created on Sun Mar 25 20:16:09 2018

@author: Vinko Kolobara
"""

import pandas as pd

df = pd.read_csv('pigs.csv')

df.set_index('Month')

df['Value'] = df['Value'].pct_change(1)

df.describe()

pd.concat([df['Value'], df['Value'].shift(-1)], axis=1).dropna().to_csv('pigs_perc.csv', index=False)

