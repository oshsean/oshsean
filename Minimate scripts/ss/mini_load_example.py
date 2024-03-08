import load_minimate_v20240219 as mini
import matplotlib.pyplot as plt
import numpy as np

directory = r'C:\SLRSOSFiles\Jobs & Projects\Coward St Mascot\240201\BE13664-BG16435\ASCII'
fname = 'BE13664_20_1_2024_0_1_10.TXT'

# read ASCII file, returns column entries in ASCII file as string
data = mini.quick_plot(fname, directory)
