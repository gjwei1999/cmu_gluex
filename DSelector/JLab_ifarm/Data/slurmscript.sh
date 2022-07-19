#!/bin/bash

###################################################################
#                                                                 #
#         Shell script to run DSelector @ifarm using SLURM        #
#            Created by Jiawei Guo @ifarm, July 2022              #
#                                                                 #
###################################################################

#SBATCH -pproduction
#SBATCH --account=halld
#SBATCH -N1
#SBATCH -n64
#SBATCH --mem-per-cpu=2000
#SBATCH -t4-00:00:00

#SBATCH -o../jobfiles/%x_%J.out


WORKDIR=/scratch/slurm_$SLURM_JOB_ID
PERMDIR=$SLURM_SUBMIT_DIR                 ## the current directory when you launch the jobn with sbatch.  Output files are copied here


## make sure that the input root tree exists
if [ ! -f $INFILE ] ; then
    echo File $INFILE not found
    exit
fi


echo ------------------------------ Variables ------------------------------
echo $RUN
echo $INFILE
echo $TREENAME
echo $DSELNAME
echo $CodeDir
echo $PROOFTHREADS
echo $OUTDIR
echo $OUT_FlatTree
echo ------------------------------ JOB_INFO  ------------------------------
echo job name is $SLURM_JOB_NAME and ID is $SLURM_JOB_ID
echo running on node $SLURM_JOB_NODELIST on queue $SLURM_JOB_PARTITION
echo
echo workdir is $WORKDIR
echo permdir is $PERMDIR
echo -----------------------------------------------------------------------



echo ------------------------------ Transfer File ------------------------------ 

echo Transferring files from server to compute node directory ${WORKDIR}


# Files needed to run the job
F_RMACRO=${CodeDir}/process_this_run.C
F_DSELC=${CodeDir}/DSelector_$DSELNAME.C
F_DSELH=${CodeDir}/DSelector_$DSELNAME.h

FILES_NEEDED="$F_RMACRO $F_DSELC $F_DSELH"


#expected output
ROOTFILE1=tree_${DSELNAME}.root  #***********
ROOTFILE2=hist_${DSELNAME}.root  #***********

ROOTFILE1_target=tree_${DSELNAME}_${RUN}.root  #***********
ROOTFILE2_target=hist_${DSELNAME}_${RUN}.root  #***********


## Copy database files.  
##cp $CCDBFILE ccdb.sqlite
##cp $RCDBFILE rcdb.sqlite
#cp $CCDBFILE .
#cp $RCDBFILE .
#echo CCDB file is $CCDBFILE
#echo RCDB file is $RCDBFILE
#echo


# Copy other files
for FILE_NEEDED in $(echo $FILES_NEEDED); do
    if [ -f ${FILE_NEEDED} ]; then
      cp ${FILE_NEEDED} .
      echo Using ${FILE_NEEDED}
    else
      echo Cannot find file ${FILE_NEEDED} !!
      exit
    fi
done

echo
echo Files in node work directory are as follows:
ls -l


source /group/halld/Software/build_scripts/gluex_env_boot_jlab.sh
gxenv

export PATH=${PWD}:$PATH
export LD_LIBRARY_PATH=${PWD}:$LD_LIBRARY_PATH

echo
echo ------------------------------ Partial Env ------------------------------
env | grep NAME
env | grep HALLD
env | grep ROOT
echo
# root -q 'process_this_run.C(8,1,"treename","dsel")'


#  ./runroot.sh $RUN 0 $TREEFILE kpkm__B4_Tree $DSELNAME root.out
#  ./runroot.sh 31057 4 tree_beam_info_31057.root kpkm__B4_Tree kinfit5_intime root.out

# need to make this:
# process_this_run.C(31049,0,"tree_kinfit100_31049.root","kpkm__B4_Tree","pidcuts")


export rootcmd=process_this_run.C\($RUN,$PROOFTHREADS,\"$INFILE\",\"$TREENAME\",\"$DSELNAME\"\)

echo "$rootcmd"
root -b -l -q $rootcmd > root.out   #redirect terminal output to a file, otherwise it gets lost
echo

 
echo DSelector terminal output:
echo
cat root.out
echo
echo Check for leftover processes:
echo "ps aux | grep $USER | grep proofserv"
ps aux | grep $USER | grep proofserv
echo
echo ls:
ls
echo


if [ -f ${ROOTFILE1} ]; then
        mv ${ROOTFILE1} ${OUT_FlatTree}/${ROOTFILE1_target}
        echo Copying ${ROOTFILE1} to ${OUT_FlatTree}/${ROOTFILE1_target}
else
        echo Cannot find file ${ROOTFILE1} !!
        exit
fi

if [ -f ${ROOTFILE2} ]; then
        mv ${ROOTFILE2} ${OUTDIR}/${ROOTFILE2_target}
        echo Copying ${ROOTFILE2} to ${OUTDIR}/${ROOTFILE2_target}
else
        echo Cannot find file ${ROOTFILE2} !!
        exit
fi
  

exit
