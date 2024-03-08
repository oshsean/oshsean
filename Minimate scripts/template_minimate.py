import load_minimate_v20240220 as mini

# NOTES:
# -----------------------------------------------------------------------
# This script will edit data points as so:
    # removes every vibration data point with an associated freq of less than 4 Hz (only in the orthogonal direction for which the frequency is less than 4 Hz)    
    # replaces overloads (***) with a value of zero
    # replaces values (ppv or freq) that are N/A with a value of zero


# INSTRUCTIONS
# -----------------------------------------------------------------------
# 0. ********* CREATE A COPY OF THIS SCRIPT ************
# 1. Change "import load_minnimate_vXXXXXXXX as mini" to the current version, e.g., "import load_minimate_v20240211 as mini"
# 2. Create a new folder and put all ASCII files for all monitors into that folder
# 3. Change the directory variable to the directory location containing the ASCII files (Line 26 or so)
# 4. In the variable 'monitorInfo', specify the minimate name. This must match the
#    minimate name provided in the ASCII file (check inside ASCII file or look at file name to determine this)
# 5. Specify morresponding warning level, halt level and location
# 6. Specify start times and end times for weekdays and weekends
# 7. Run file. PCPV vs time charts will save in a folder called Figure within the working directory.

# INPUTS
# ----------------------------------------------------
# working directory containing all ASCII files
directory = r'\\au.slr.local\Corporate\Projects-SLR\610-SrvSYD\610-SYD\610.019446.00001 Coward Street Mascot\06 SLR Data\02 Site Notes\14 Feb - 21 Feb 24\240221\Combined ASCII'

# dictionary stucture: {'monitor': [warningLevel, haltLevel, 'Location']
# first number in [] is the operator warning level in mm/s
# second number in [] is the operator halt level in mm/s
# third value in [] is the location name as a string
# fourth value in [] is the standard for assessment:
    
    # Standard: Enter a number based on the following:
    # 1. BS 7385 Line 1 (Reinforced or framed structures, industial and heavy commercial buildings)
    # 2. BS 7385 Line 2 (Unreinforced or light framed structures, residential or light commercial type buildings)
    # 3. DIN 4150-3:2006 short-term vibration Line 1 (Commercial, industrial)
    # 4. DIN 4150-3:2006 short-term vibration Line 2 (Residential)
    # 5. DIN 4150-3:2006 short-term vibration Line 3 (Heritage, sensitive, great intrinsic value) 
        
monitor_info = {#'BE19735': [6, 7.5, 'Location 1 - Pump Station', 2],
               # 'BE14040': [6, 7.5, 'Location 2 - 165 Coward St, Mascot (Apartments)', 2],
                # 'BE17408': [6, 7.5, 'Location 3 - 161 Coward St, Mascot', 2]
                'BE13664': [6, 7.5, 'Location 1 - Pump Station', 2],
                'BE16019': [20, 25, 'Location 2 - 208 Coward St, Mascot', 2],
               }

# Specify weekday and weekend start and end times in 24 hour HH:mm format as a string
# e.g. start time 7 AM, weekdayStartTime = '7:00'
# e.g. end time 5 PM, weekdayEndTime = '17:00'
# weekdayStartTime = '0:00'
# weekdayEndTime = '23:59'
# weekendStartTime = '0:00'
# weekendEndTime = '23:59'


# PLOTTING INPUTS
# ----------------------------------------------------
# Plotting mode:
# use 'all' to plot daily charts for ALL monitors in a single figure
# use 'single' to plot daily charts for ONE monitor in a single each figure
plot_mode = 'all'

# Show plots? True to show, False to hide
# False is recommended if daily plots
show_plots = True

# Save plots? True to save, False to not save
save_plots = True

# Save images in a .docx file? True to save, False to not save
save_docx = True

# After the monitor name, specify the maximum PPV value for the y-axis (PPV axis)
# default max PPV value is set to 1.5 times the Operator Warning Level
max_plot_PPV = {#'BE14040': 10,
                #'BE19735': 10,
                #'BE17408': 10
                'BE13664': 10,
                'BE16019': 30
               }
# ---------------------------------------------------------------------------
# execute steps using inputs to read minimate data - DO NOT REMOVE !!!

# look at Variable Explorer to see summary of exceedances, daily maximum values,
# collection of monitoring data and list of monitoring dates

exceedance_summary, daily_maximums, data, list_of_dates = \
mini.steps(monitor_info, directory,
           plot_mode, show_plots, save_plots, save_docx, max_plot_PPV) 