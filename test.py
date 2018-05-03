#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  3 09:37:53 2018

@author: vkolobara
"""

import pandas as pd
from preprocessing.time_series.fuzzify_dataset import *

df = pd.read_csv('data/iris.data')


print(df.columns)