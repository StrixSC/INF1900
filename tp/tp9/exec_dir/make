#!/bin/bash


#Simplement ecrire ./make.sh dans le exec_dir  et le programme executera toute les commandes necessaire pour compiler la librairie et les executables.

echo "
 ____   ____    ___    ____  ___ ___    ___  ___ ___ 
|    \ |    \  /   \  /    ||   |   |  /  _]|   |   |
|  o  )|  D  )|     ||   __|| _   _ | /  [_ | _   _ |
|   _/ |    / |  O  ||  |  ||  \_/  ||    _]|  \_/  |
|  |   |    \ |     ||  |_ ||   |   ||   [_ |   |   |
|  |   |  .  \|     ||     ||   |   ||     ||   |   |
|__|   |__|\_| \___/ |___,_||___|___||_____||___|___|
                                                     
"
./progmem -v -o output.bin write/progmemTest.txt
# ./progmem -v -o output.bin write/progmemSONG.txt
make clean -C write
make -C write
make install -C write

echo " 
  _____ ______   ____  ____   ______  ____  ____    ____      ____     ___   ____  ___   
 / ___/|      | /    ||    \ |      ||    ||    \  /    |    |    \   /  _] /    ||   \  
(   \_ |      ||  o  ||  D  )|      | |  | |  _  ||   __|    |  D  ) /  [_ |  o  ||    \ 
 \__  ||_|  |_||     ||    / |_|  |_| |  | |  |  ||  |  |    |    / |    _]|     ||  D  |
 /  \ |  |  |  |  _  ||    \   |  |   |  | |  |  ||  |_ |    |    \ |   [_ |  _  ||     |
 \    |  |  |  |  |  ||  .  \  |  |   |  | |  |  ||     |    |  .  \|     ||  |  ||     |
  \___|  |__|  |__|__||__|\_|  |__|  |____||__|__||___,_|    |__|\_||_____||__|__||_____|
                                                                                         
"
./serieViaUSB -e -f output.bin
make clean
make
make install
od -v -t x1 output.bin
