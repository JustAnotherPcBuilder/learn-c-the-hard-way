#!/bin/bash


for i in $(seq 0 99)
do
    ./ex17-2 test.db s "$i" "$i" "$i@gmail.com" 
done