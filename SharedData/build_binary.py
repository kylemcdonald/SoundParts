import argparse
from os.path import basename, splitext

import numpy as np
from scipy.io import wavfile
from list_all_files import list_all_files
from ffmpeg_load_audio import ffmpeg_load_audio 

parser = argparse.ArgumentParser(
    description='Convert folder of samples into binary files.')
parser.add_argument('-i', '--input_dir', type=str, default='samples')
args = parser.parse_args()

def load_audio(fn):
	base, ext = splitext(fn)
	if ext == '.wav':
		return wavfile.read(fn)[1]
	else:
		return ffmpeg_load_audio(fn, mono=True)[0]

filenames = list(list_all_files(args.input_dir, ['.wav', '.mp3']))
samples = [load_audio(fn) for fn in filenames]
samples = np.asarray(samples, dtype=np.int16)
samples.tofile(args.input_dir + '.audio.bin')
print('Saved samples: {}'.format(samples.shape))

pitches = [int(basename(fn).split('-')[1]) for fn in filenames]
pitches = np.asarray(pitches, dtype=np.float32)
pitches.tofile(args.input_dir + '.meta.bin')
print('Saved pitches: {}'.format(pitches))
