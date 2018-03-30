#!/usr/bin/env python3

from time_series import TimeSeries as TS
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('../../data/pigs.csv')

ts = TS(df, 'Value')

ts.plot()

target_col = ts.get_target()
change = ts.pct_change()
pct_change.plot()