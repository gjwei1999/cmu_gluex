#!/bin/bash

###################################################################
#                                                                 #
#    Shell script to loop over multiple files @CMU using SLURM    #
#                 Created by Jiawei Guo @CMU, Sept 2021           #
#                                                                 #
###################################################################


#------------------------------ Varaibles ------------------------------

# Declare any process name i.e. pi0pi0pippim
MyProcess=ggkpkm_thrown

# Input pathway to DSelector and runSelector of choice
CodeDir=/home/jiaweigu/cmu_gluex/2017_MC/ggkpkm/kpkmpi0_dsel/thrown/dsel

# Input pathway to desired TTrees
MyDataInDir=/raid3/jiaweigu/mc_sim/gen_amp/flat_kpkmpi0_for2017/thrown

# Input specific TTree base name i.e. tree_pi0pi0pippim__B4
TREENAME=Thrown_Tree

# Input DSelector name i.e. DSelector_ggkpkm.C/h
DSELNAME=ggkpkm__B4_thrown

# how many threads to use
PROOFTHREADS=0
         
# Input pathway to /raid directory where histogram/TTree/FlatTree file will go
# (must specify atleast one pathway)
#
#          Example 
#
#          MyDataOutDirVar=pi0pi0pippim__B4
#          MyDataOutDir=/raid4/zbaldwin/RunPeriod-2018-08/${MyDataOutDirDir}/
 
OUTDIR=/home/jiaweigu/cmu_gluex/2017_MC/ggkpkm/kpkmpi0_dsel/thrown/output_nocut

#MyTreeOutDir=
#MyFlatTreeOutDir=

#----------------------------------------------------------------------

mkdir -p $OUTDIR


echo These are the pathways that were specified
echo "Input Directory: $MyDataInDir"
echo "Output Histogram Directory: $OUTDIR"
echo 
echo


echo "Submitting Jobs:"
echo
echo "Do you want to submit specific run-numbers or all run-numbers? "
echo "Hit space for speccifc run-numbers, type 'all' for every run-number"
read Varname

if [[ $Varname == "all" ]]; then 
    
    # Will now gather the run-number associated in the name of TTree
    
    #FOR ALL THE RUN NUMBERS
    for INFILE in ${MyDataInDir}/*; do
   
    echo $INFILE 
    

    RUN=`basename ${INFILE}`

        #________ FOR THE TTREES ____________#
        # Must update for specfic TTree filenames
    RUN=${RUN:21:9}  # this gets run number from filename   Ex.  tree_thrown_gen_amp_030693_001.root tree_thrown_ggkpkm__B4_030274.root
    #RUN=1           #                                           123456789012345678901  so 24 letters in name until the 3
    
    # This echo will make sure you are getting out the correct 5 run-numbers associated with the TTree file
    echo $RUN
    
    # Submits a unqie job into SLURM
    sbatch --job-name=${MyProcess}_${RUN} --export=RUN=$RUN,INFILE=$INFILE,TREENAME=$TREENAME,DSELNAME=$DSELNAME,CodeDir=$CodeDir,PROOFTHREADS=$PROOFTHREADS,OUTDIR=$OUTDIR slurmscript_thrown.sh
    
    # In the past SLURM had issues, this sleep command fixed that (ISSUE RESOLVED JAN 2020)
    #sleep $SLEEP
  
    # Normally exclude one node for other peoples use

    done

else

    #INFILE="/raid12/jiaweigu/RunPeriod-2017-01/analysis/ver20/ggkpkm__B4_Tree/tree_ggkpkm__B4_030274.root"
    INFILE="/raid13/gluex/test/trees/tree_ggkpkm__B4.root"
    RUN="1"

    sbatch --job-name=${MyProcess}_${RUN} --export=RUN=$RUN,INFILE=$INFILE,TREENAME=$TREENAME,DSELNAME=$DSELNAME,CodeDir=$CodeDir,PROOFTHREADS=$PROOFTHREADS,OUTDIR=$OUTDIR slurmscript.sh
fi





exit 1



