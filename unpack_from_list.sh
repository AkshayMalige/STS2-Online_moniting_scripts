#!/bin/bash

for f in $(cat $1); do ./quick_ana.sh $f; done
