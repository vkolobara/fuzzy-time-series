#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jun 17 11:01:37 2018

@author: vkolobara
"""

import pandas as pd
from matplotlib import pyplot as plt
import os
'''
curr_max = 0
best = ''
data = []

for file in os.listdir('log'):
    if '.csv' not in file or 'iotusd' not in file:
        continue
    df = pd.read_csv(os.path.join('log', file), index_col=False)['Test']
    df_t = pd.read_csv(os.path.join('log', file), index_col=False)['Train']
    
    xs = [(max(df.iloc[i-100:i]) + max(df_t.iloc[i-100:i])) / 2 for i in range(100, len(df), 100)]
    data.append(xs)
    mn = sum(xs) / len(xs)
    print(mn, file)
    if mn > curr_max:
        best = file
        curr_max = mn

plt.xlabel('Parametri')
plt.ylabel('Dobrota')
plt.boxplot(data, vert=True)
plt.savefig('parameter_selection.pdf')
'''

df = pd.read_csv('data/raw/pigs.csv')
#plt.figure()
#plt.xlabel('Vremenski korak')
#plt.ylabel('Broj zaklanih svinja')
#plt.plot(df['Value'])    
#plt.savefig('pigs.pdf')

BOUGHT: 20 21 23 24 27 31 38 54 82 
SOLD: 38 134 135 140 141 146 148 153 160 274 314