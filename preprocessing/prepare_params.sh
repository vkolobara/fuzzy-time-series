for f in /home/vkolobara/git/fuzzy-time-series/data/train/*;
do
    if [[ $f == *ohlc* ]]; then
        python create_parameters.py $f;
    fi
done
