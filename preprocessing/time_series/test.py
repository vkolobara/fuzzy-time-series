# -*- coding: utf-8 -*-
"""
Created on Thu Apr 12 11:48:41 2018

@author: Vinko Kolobara
"""

import pandas as pd
from fin_time_series.financial_time_series import *

df = pd.read_csv("../../data/SP500.csv")
df = df.set_index("Date")

moving_average(df['Close']).plot()


