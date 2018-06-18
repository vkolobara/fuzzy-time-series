#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import pandas as pd
from matplotlib import pyplot as plt
import os

curr_max = 0
best = ''

for file in os.listdir('log'):
    if '.csv' not in file:
        continue
    df = pd.read_csv(os.path.join('log', file), index_col=False)['Test']
    df_t = pd.read_csv(os.path.join('log', file), index_col=False)['Train']
    
    df1 = df.iloc[:600]
    df2 = df.iloc[600:1200]
    df3 = df.iloc[1200:1800]
    df4 = df.iloc[1800:2400]
    df5 = df.iloc[2400:]
    
    mn = sum([df1.max(), df2.max(), df3.max(), df4.max(), df5.max()]) / 5
    mn = (mn + df_t.max())/2
    if mn > curr_max:
        best = file
        curr_max = mn
        

    