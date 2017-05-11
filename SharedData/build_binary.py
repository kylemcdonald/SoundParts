import argparse
from os.path import basename, splitext, normpath

import numpy as np
from scipy.io import wavfile
from list_all_files import list_all_files
from ffmpeg_load_audio import ffmpeg_load_audio 

parser = argparse.ArgumentParser(
    description='Convert folder of samples into binary files.')
parser.add_argument('-i', '--input_dir', type=str, default='samples')
args = parser.parse_args()
args.input_dir = normpath(args.input_dir)

def load_audio(fn):
	base, ext = splitext(fn)
	if ext == '.wav':
		return wavfile.read(fn)[1]
	else:
		return ffmpeg_load_audio(fn, mono=True)[0]

filenames = list(list_all_files(args.input_dir, ['.wav', '.mp3']))
pitches = [int(basename(fn).split('-')[1]) for fn in filenames]
pitches, filenames = zip(*sorted(zip(pitches, filenames)))
samples = [load_audio(fn) for fn in filenames]
samples = np.asarray(samples, dtype=np.int16)
samples.tofile(args.input_dir + '.bin')
print('Saved samples: {}'.format(samples.shape))

np.savetxt(args.input_dir + '.txt', pitches, fmt='%d')
print('Saved pitches: {}'.format(len(pitches)))