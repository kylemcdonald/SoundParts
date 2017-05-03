import os
import fnmatch

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