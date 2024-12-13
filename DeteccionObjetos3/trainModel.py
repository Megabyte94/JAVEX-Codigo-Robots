import os
import cv2

# Replace with paths to your positive and negative samples
positive_file = 'pos_samples.vec'
negative_file = 'neg.lst'
output_directory = 'classifier'

# Configure parameters for training
numPos = 800  # Number of positive samples
numNeg = 400  # Number of negative samples
numStages = 10
width = 24
height = 24

# Command for training the cascade classifier
command = f'opencv_traincascade -data {output_directory} -vec {positive_file} -bg {negative_file} -numPos {numPos} -numNeg {numNeg} -numStages {numStages} -w {width} -h {height}'

# Run the command
os.system(command)
