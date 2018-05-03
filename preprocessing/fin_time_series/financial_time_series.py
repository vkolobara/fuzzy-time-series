import pandas as pd

def moving_average(df, n):
    """
    Calculate moving average with window n.
    """
    ma = pd.Series(df['Close'].rolling(n, min_periods=n).mean(), name='MA_' + str(n))
    return df.join(ma)

def exponential_moving_average(df, n):
    """
    Calculates exponential moving average with window n.
    """
    ema = pd.Series(df['Close'].ewm(span=n, min_periods=n).mean(), name='EMA_' + str(n))
    return df.join(ema)

def bollinger_bands(df, n):
    """
    Calculate bollinger bands with window n.
    Bollinger bands are 2 standard deviations away from a normal moving
    average.
    """
    ma = pd.Series(df['Close'].rolling(n, min_periods=n).mean())
    msd = pd.Series(df['Close'].rolling(n, min_periods=n).std())
    b1 = 4 * msd / ma
    B1 = pd.Series(b1, name='BollingerB1_' + str(n))
    ret = df.join(B1)

    b2 = (df['Close'] - ma + 2 * msd) / (4 * msd)
    B2 = pd.Series(b2, name='BollingerB2_' + str(n))
    return ret.join(B2)

def stochastic_oscillator_k(df):
    """
    Calculates stochastic_oscillator_k for current prices.
    """
    sok = pd.Series((df['Close'] - df['Low']) / (df['High'] - df['Low']), name='SOk')
    return df.join(sok)

def stochastic_oscillator_d(df, n):
    """
    Calculate exponential moving average of stochastic oscillator.
    """
    sok = pd.Series((df['Close'] - df['Low']) / (df['High'] - df['Low']), name='SOk')
    sod = pd.Series(sok.ewm(span=n, min_periods=n).mean(), name='SOd_' + str(n))
    return df.join(sod)

def macd(df, n_fast, n_slow):
    """
    Calculates MACD with n_fast and n_slow as parameters for number of periods.
    """
    ema_fast = pd.Series(df['Close'].ewm(span=n_fast, min_periods=n_slow).mean())
    ema_slow = pd.Series(df['Close'].ewm(span=n_slow, min_periods=n_slow).mean())
    macd = pd.Series(ema_fast - ema_slow, name='MACD_' + str(n_fast) + '_' + str(n_slow))
    macd_signal = pd.Series(macd.ewm(span=9, min_periods=9).mean(), name='MACDSign_' + str(n_fast) + '_' + str(n_slow))
    macd_diff = pd.Series(macd - macd_signal, name='MACDDiff_' + str(n_fast) + '_' + str(n_slow))
    return df.join(macd).join(macd_signal).join(macd_diff)
