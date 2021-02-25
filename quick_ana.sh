#!/bin/bash

fn=$1

go4analysis -user /home/akshay/feb21_beam/dataset/$fn -asf ./$(basename $fn .hld).root -number 10000

