import sys
import os.path
import subprocess
import math

class Box:
    def __init__(self, x1, y1, x2, y2):
        if math.isnan(x1) == False:
            self.x1 = x1
            self.y1 = y1
            self.x2 = x2
            self.y2 = y2
            self.isFailed = False
        else:
            self.isFailed = True
    def __str__(self):
    	return "Box(" + str(self.x1) + "," + str(self.y1) + ")"

    def __eq__(self, other):
        return (self.isFailed and other.isFailed ) or (self.x1 == other.x1 and self.y1 == other.y1 and self.x2 == other.x2 and self.y2 == other.y2)


def construct_box_list(file_path):
    box_file = open(file_path, "r")
    lines = box_file.readlines()
    nrOfLines = len(lines)
    boxList = []
    for line in lines:
    	params = line.split(",")
    	x1 = float(params[0].strip(" "));
     	y1 = float(params[1].strip(" "));
     	x2 = float(params[2].strip(" "));
    	y2 = float(params[3].strip(" "));
        b = Box(x1, y1, x2, y2)
        boxList.append(b)
    return boxList

def compare_tld_mtld(datasetPath, nrOfFrames):
    tld_file = datasetPath + "/evaluations/TLD1.0.txt"
    mtld_file = datasetPath + "/evaluations/mTLD.txt"

    tld_box_list = construct_box_list(tld_file)
    mtld_box_list = construct_box_list(mtld_file)

    for i in range(0, nrOfFrames):
        tld_box = tld_box_list[i];
        mtld_box = mtld_box_list[i];

        if tld_box == mtld_box :
            print "Frame(" + str(i) + ") is OK"
        else:
            print str(tld_box) + " is not equal " + str(mtld_box)


resources_dir = "/Users/mehmetgunturkun/MasterThesis/resources"


target_dir = "../target/"
multitld_command = "./tld-one-test-app"

dataset = sys.argv[1]
print "Dataset(" + dataset + ") is going to processed"

datasetPath = resources_dir + "/" + dataset
sequencePath = datasetPath + "/" + "sequence"
nrOfFrames = len(os.listdir(sequencePath))
print "There are " + str(nrOfFrames) + " frames under " + dataset

compare_tld_mtld(datasetPath, 100)

# p = subprocess.Popen([multitld_command, "--key", dataset, "--limit", "10"], cwd=target_dir)
# p.wait()

# p = subprocess.Popen([multitld_command, dataset] cwd=target_dir)
# p.wait()
