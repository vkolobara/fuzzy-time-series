for f in /home/vkolobara/git/fuzzy-time-series/parameters/*;
do
    if [[ $f == *21600_10_100_0.9_0.7* ]]; then
        ./build/fts-financial $f;
    fi
done
