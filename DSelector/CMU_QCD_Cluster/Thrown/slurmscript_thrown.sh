#!/bin/bash

###################################################################
#                                                                 #
#         Shell script to run DSelector @CMU using SLURM          #
#                 Created by Jiawei Guo @CMU, Sept 2021           #
#                                                                 #
###################################################################

#SBATCH -pblue
#SBATCH -N1
#SBATCH -n64
#SBATCH --mem-per-cpu=1000M
#SBATCH -t0-03:00:00
#SBATCH -otest_%J.out

##SBATCH --mail-type=END  
##SBATCH --mail-user=jiaweigu@andrew.cmu.edu

#N number of nodes, n: number of total processes


WORKDIR=/scratch/slurm_$SLURM_JOB_ID
PERMDIR=$SLURM_SUBMIT_DIR                 ## the current directory when you launch the jobn with sbatch.  Output files are copied here


CCDBFILE=/home/gluex2/gluexdb/ccdb_2022_07_15.sqlite
RCDBFILE=/home/gluex2/gluexdb/rcdb_2022_07_15.sqlite



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
echo ------------------------------ JOB_INFO  ------------------------------
echo job name is $SLURM_JOB_NAME and ID is $SLURM_JOB_ID
echo running on node $SLURM_JOB_NODELIST on queue $SLURM_JOB_PARTITION
echo
echo workdir is $WORKDIR
echo permdir is $PERMDIR
echo -----------------------------------------------------------------------


cd $WORKDIR

echo ------------------------------ Transfer File ------------------------------ 

echo Transferring files from server to compute node directory ${WORKDIR}
cd ${WORKDIR}

echo Copying $INFILE
/usr/bin/scp -l 10000 $INFILE .      # limit scp to 10 MB/s
echo

# Files needed to run the job
F_RMACRO=${CodeDir}/process_thrown_run.C
F_DSELC=${CodeDir}/DSelector_$DSELNAME.C
F_DSELH=${CodeDir}/DSelector_$DSELNAME.h

FILES_NEEDED="$F_RMACRO $F_DSELC $F_DSELH"


#expected output
ROOTFILE1=tree_${DSELNAME}.root  #***********
ROOTFILE2=hist_${DSELNAME}.root  #***********

ROOTFILE1_target=tree_${DSELNAME}_${RUN}.root  #***********
ROOTFILE2_target=hist_${DSELNAME}_${RUN}.root  #***********


## Copy database files.  
#cp $CCDBFILE ccdb.sqlite
#cp $RCDBFILE rcdb.sqlite
cp $CCDBFILE .
cp $RCDBFILE .
echo CCDB file is $CCDBFILE
echo RCDB file is $RCDBFILE
echo


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

#source /home/gluex2/gluex_top/gluex_env_boot.csh
export GLUEX_TOP=/home/gluex2/gluex_top
export HALLD_VERSIONS=$GLUEX_TOP/halld_versions
export BUILD_SCRIPTS=/home/gluex2/gluex_top/build_scripts
source $BUILD_SCRIPTS/gluex_env_version.sh /home/gluex2/gluex_top/halld_versions/version_5.4.1.xml


ln -s /usr/bin/python2 python

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


TREEFILE="${INFILE##*/}"        ## last part of input file path

export rootcmd=process_thrown_run.C\(\"$RUN\",$PROOFTHREADS,\"$TREEFILE\",\"$TREENAME\",\"$DSELNAME\"\)

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

#for FILE_NEEDED in $ROOTFILE1 $ROOTFILE2; do
#for FILE_NEEDED in $ROOTFILE2; do 
#    if [ -f ${FILE_NEEDED} ]; then
#      cp ${FILE_NEEDED} $OUTDIR
#      echo Copying ${FILE_NEEDED} to $OUTDIR
#    else
#      echo Cannot find file ${FILE_NEEDED} !!
#      exit
#    fi
#done

if [ -f ${ROOTFILE1} ]; then
        cp ${ROOTFILE1} ${OUTDIR}/${ROOTFILE1_target}
        echo Copying ${ROOTFILE1} to ${OUTDIR}/${ROOTFILE1_target}
else
        echo Cannot find file ${ROOTFILE1} !!
        exit
fi

if [ -f ${ROOTFILE2} ]; then
        cp ${ROOTFILE2} ${OUTDIR}/${ROOTFILE2_target}
        echo Copying ${ROOTFILE2} to ${OUTDIR}/${ROOTFILE2_target}
else
        echo Cannot find file ${ROOTFILE2} !!
        exit
fi
  

exit
