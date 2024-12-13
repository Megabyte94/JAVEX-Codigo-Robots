import os

# Folder paths for positive and negative images
positive_folder = 'p'
negative_folder = 'n'

# Output files
positive_file = 'pos.lst'
negative_file = 'neg.lst'

# Generate pos.lst for positive images
with open(positive_file, 'w') as pos_file:
    for filename in os.listdir(positive_folder):
        if filename.endswith('.jpg') or filename.endswith('.png'):
            # Each line includes the image path and the bounding box details (for training)
            pos_file.write(f'{positive_folder}/{filename} 1 0 0 38 38\n')

# Generate neg.lst for negative images
with open(negative_file, 'w') as neg_file:
    for filename in os.listdir(negative_folder):
        if filename.endswith('.jpg') or filename.endswith('.png'):
            neg_file.write(f'{negative_folder}/{filename}\n')

print(f'{positive_file} and {negative_file} generated successfully!')
