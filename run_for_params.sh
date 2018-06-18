for f in /home/vkolobara/git/fuzzy-time-series/parameters/*;
do
    if [[ $f == *iotusd_86400_10* ]]; then
        ./build/fts-financial $f;
    fi
done
