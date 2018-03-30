import pandas as pd
from matplotlib import pyplot as plt

class TimeSeries(object):
    def __init__(self, df, target_column_name):
        self.df = df
        self.target_column_name = target_column_name
    
    def get_target(self):
        return self.df.shift(-1)[self.target_column_name]

    def moving_average(self, window):
        return self.df.rolling(window=window).mean()    

    def exponential_moving_average(self, span):
        return pd.ewma(self.df, span=span)

    def pct_change(self, periods=1):
        return self.df.pct_change(periods=periods)
    
    def diff(self, order=1):
        return self.df.diff(order)

    def plot(self):
        self.df.plot()