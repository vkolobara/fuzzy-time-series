for f in /home/vkolobara/git/fuzzy-time-series/parameters/*;
do
    if [[ $f == *btcusd*21600* ]]; then
        ./build/fts-financial $f;
    fi
done
