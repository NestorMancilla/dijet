#!/bin/bash

# get runs: grep ":" fullrun.json|cut -d \" -f 2
runs=()

brilcalc trg --hltpath HLT_PFJet* --prescale --output-style csv -r <run>
