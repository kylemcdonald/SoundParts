import argparse
import fnmatch
import os
from os.path import basename

import numpy as np
from scipy.io import wavfile

# extensions can be a single tring like '.png' or '.jpg'
# or a list of extensions. they should all be lowercase
# but the . is important.
def list_all_files(directory, extensions=None):
    for root, dirnames, filenames in os.walk(directory):
        for filename in filenames:
            base, ext = os.path.splitext(filename)
            joined = os.path.join(root, filename)
            if extensions is None or ( len(ext) and ext.lower() in extensions ):
                yield joined

parser = argparse.ArgumentParser(
    description='Convert folder of samples into binary files.')
parser.add_argument('-i', '--input_dir', type=str, default='samples')
args = parser.parse_args()

filenames = list(list_all_files(args.input_dir, '.wav'))
samples = [wavfile.read(fn)[1] for fn in filenames]
samples = np.asarray(samples, dtype=np.int16)
samples.tofile(args.input_dir + '.audio.bin')
print('Saved samples: {}'.format(samples.shape))

pitches = [int(basename(fn).split('-')[1]) for fn in filenames]
pitches = np.asarray(pitches, dtype=np.float32)
pitches.tofile(args.input_dir + '.meta.bin')
print('Saved pitches: {}'.format(pitches))
