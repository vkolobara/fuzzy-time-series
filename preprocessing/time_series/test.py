# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd

df = pd.read_csv("pigs.csv")
df = df.set_index("Month")

df['Value_out'] = df['Value'].shift(-1)
df = df.dropna()
df.Value_out = df.Value_out.astype(int)

print(df.dtypes)

df.to_csv("pigs_prep.csv")