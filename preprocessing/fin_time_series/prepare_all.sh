for f in /home/vkolobara/Downloads/crypto_data/crypto_data/*;
do
    python create_dataset.py $f;
done
