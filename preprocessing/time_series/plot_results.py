import matplotlib.pyplot as plt
import pandas as pd
import argparse

def file_to_pandas(file_path):
    return pd.read_csv(file_path)

def plot_time_series(real_data, pred_data, join_col):
    df = real_data.set_index(join_col).join(pred_data.set_index(join_col), how='outer', lsuffix='_real', rsuffix='_pred')
    df.plot()
    plt.show()

def main():
    parser = argparse.ArgumentParser(description='Plotting time series chart.')
    parser.add_argument('real_data', type=str, help='path to the file which contains raw time series data')
    parser.add_argument('pred_data', type=str, help='path to the file which contains predicted time series data')
    args = parser.parse_args()

    plot_time_series(file_to_pandas(args.real_data), file_to_pandas(args.pred_data), join_col="Month")

if __name__ == '__main__':
    main()