#!/bin/bash

# Configured to pick golden JSONs hand-edited to match separate IOVs
# usage: bash runbril <triggerpaths in txt> <yr> <IOV>

trigs=../$1
certjson="jsonfiles/Cert_Collisions"$2$3.json
#certjson="jsonfiles/Cert_Collisions2023_366442_370790_Golden.json"
pileupfile="jsonfiles/pileupJSON"$2".txt"
normtag="jsonfiles/normtag_BRIL.json"

mbiasxsec=80000

echo "Using lumi json "$certjson
echo "Using pileup json "$pileupfile

outdir=""
outdir=${2}${3}

mkdir -p ${outdir}
cd ${outdir}

while IFS= read -r hpath

do
#    echo $hpath
    hltpath=$hpath"_v*"
    echo $hltpath

    brilcalc lumi --byls --normtag ../${normtag} --minBiasXsec ${mbiasxsec} -u /fb --hltpath "$hltpath" -i ../"$certjson" -o "$hpath"_out.csv
    wait
    echo "Start pileupReCalc"
    pileupReCalc_HLTpaths.py -i ${hpath}_out.csv --inputLumiJSON ../${pileupfile} -o ${hpath}_pu.txt --runperiod Run3
    wait
    echo "Start pileUpCalc"
    pileupCalc.py -i ../"$certjson" --inputLumiJSON ${hpath}_pu.txt --calcMode true --minBiasXsec ${mbiasxsec} --maxPileupBin 120 --numPileupBins 120 pu_${hpath}.root
    wait

done < "$trigs"

cd ..
