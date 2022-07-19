#!/bin/bash

###################################################################
#                                                                 #
#    Shell script to loop over multiple files @CMU using SLURM    #
#                 Created by Jiawei Guo @CMU, Sept 2021           #
#                                                                 #
###################################################################


#------------------------------ Varaibles ------------------------------

# Declare any process name i.e. pi0pi0pippim
MyProcess=ggkpkm

# Input pathway to DSelector and runSelector of choice
CodeDir=/w/halld-scshelf2101/home/gjwei/2017_Data/ggkpkm/ver20/dsel_cut

# Input pathway to desired TTrees
MyDataInDir=/lustre19/expphy/cache/halld/RunPeriod-2017-01/analysis/ver20/tree_ggkpkm__B4/merged

# Input specific TTree base name i.e. tree_pi0pi0pippim__B4
TREENAME=ggkpkm__B4_Tree

# Input DSelector name i.e. DSelector_ggkpkm.C/h
DSELNAME=ggkpkm__B4

# how many threads to use
PROOFTHREADS=8 
         
# Input pathway to /raid directory where histogram/TTree/FlatTree file will go
# (must specify atleast one pathway)
#
#          Example 
#
#          MyDataOutDirVar=pi0pi0pippim__B4
#          MyDataOutDir=/raid4/zbaldwin/RunPeriod-2018-08/${MyDataOutDirDir}/
 
OUTDIR=/w/halld-scshelf2101/home/gjwei/2017_Data/ggkpkm/ver20/out_hist
OUT_FlatTree=/lustre19/expphy/volatile/halld/home/gjwei/2017_Data_after_cut

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
    	RUN=${RUN:17:5}  # this gets run number from filename   Ex.  tree_ggkpkm__B4_040856.root
           	             #                                           12345678901234567  so 17 letters in name until the 3
    
    	# This echo will make sure you are getting out the correct 5 run-numbers associated with the TTree file
    	echo $RUN
 
    	mkdir submit_${RUN}
    	cp slurmscript.sh submit_${RUN}
		cd submit_${RUN}  
 
    	# Submits a unqie job into SLURM
    	sbatch --job-name=${MyProcess}_${RUN} --export=RUN=$RUN,INFILE=$INFILE,TREENAME=$TREENAME,DSELNAME=$DSELNAME,CodeDir=$CodeDir,PROOFTHREADS=$PROOFTHREADS,OUTDIR=$OUTDIR,OUT_FlatTree=$OUT_FlatTree slurmscript.sh

    	cd ..
    
    	# In the past SLURM had issues, this sleep command fixed that (ISSUE RESOLVED JAN 2020)
    	#sleep $SLEEP
  
    	# Normally exclude one node for other peoples use

		done

else

    INFILE="/raid12/jiaweigu/RunPeriod-2017-01/analysis/ver20/ggkpkm__B4_Tree/tree_ggkpkm__B4_030274.root"
    RUN="30274"

    sbatch --job-name=${MyProcess}_${RUN} --export=RUN=$RUN,INFILE=$INFILE,TREENAME=$TREENAME,DSELNAME=$DSELNAME,CodeDir=$CodeDir,PROOFTHREADS=$PROOFTHREADS,OUTDIR=$OUTDIR slurmscript.sh
fi





exit 1



