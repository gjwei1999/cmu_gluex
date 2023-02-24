#!/usr/bin/env python3
import argparse
from pathlib import Path
import sys
import os



def main(args):

    path = Path(args.path)

    Pol_name_list = ['PARA_0', 'PERP_45', 'PERP_90', 'PARA_135']
    dataset_list = ['S17', 'S18', 'F18']
    hist_name_list = {'PARA_0':'hPol0', 'PERP_45':'hPol45', 'PERP_90':'hPol90', 'PARA_135':'hPol135'}

    pol_info_S17 = {"path": '/home/jiaweigu/cmu_gluex/PolValues/makePolValsV9/outFiles/sp17TPol.root',
                "PARA_0": 1.8,
                "PERP_45": 47.9,
                "PERP_90": 94.5,
                "PARA_135": -41.6}
                
    pol_info_S18 = {"path": '/home/jiaweigu/cmu_gluex/PolValues/makePolValsV9/outFiles/sp18TPol75.root',
                "PARA_0": 4.1,
                "PERP_45": 48.5,
                "PERP_90": 94.2,
                "PARA_135": -42.4}

    pol_info_F18 = {"path": '/home/jiaweigu/cmu_gluex/PolValues/makePolValsV9/outFiles/fa18TPol.root',
                "PARA_0": 3.3,
                "PERP_45": 48.3,
                "PERP_90": 92.9,
                "PARA_135": -42.1}

    for file in path.iterdir():
        # skip the AMO file
        if 'AMO' in file.name:
            continue

        dataset_info = [sub for sub in dataset_list if sub in file.name]
        if len(dataset_info) != 1:
            print('Error! The file name contains more than one S17, S18, F18')
            sys.exit(1)
        if dataset_info[0] == 'S17':
            pol_info = pol_info_S17
        elif dataset_info[0] == 'S18':
            pol_info = pol_info_S18
        elif dataset_info[0] == 'F18':
            pol_info = pol_info_F18

        polAng_string = [sub for sub in Pol_name_list if sub in file.name]
        if len(polAng_string) != 1:
            print('Error! The file name contains more than one PARA_0, PERP_45, PERP90, PARA_135')
            sys.exit(1)
        
        polAng = pol_info.get(polAng_string[0])
        hist_name = hist_name_list.get(polAng_string[0])
        pol_Ebeam_hist_path = pol_info['path']

        print()
        print("------- Start processing "+file.name+" -------")
        
        command = ['/home/jiaweigu/cmu_gluex/Utilities/polVal_to_amptoolTree/polVal_to_amptoolTree ', str(file.absolute().parent), str(file.name), str(polAng), pol_Ebeam_hist_path, hist_name]
        #print(' '.join(command))
        os.system(' '.join(command))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    description = '''This script adds polarization information to beam photons 4-vector in the amptool-format trees. File name should include information like "PARA_0" and "S17". '''
    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-p', '--path', help="path to amptoolTree files.")
    
    args = parser.parse_args()
    main(args)