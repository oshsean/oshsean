import numpy as np; import os; import pandas as pd; import matplotlib.pyplot as plt; import datetime as dt; import matplotlib.dates as mdates; import docx; from docx.shared import Cm

# TO DO:
# -------------------------------------
# Edit script so that it finds the line containing the date in the Minimate file
    # and starts reading the data from there 
# Export and order daily maximum values to docx
# Export summary table to docx
# Implement daylight savings mode by specifying start date and end day for time shift
    # for both forward or backward clock

# FUNCTIONS
# -----------------------------------------------------------------------
def steps(monitor_info, directory,
          plot_mode, show_plots, save_plots, save_docx, max_plot_PPV):
    
    global data_storage, workingDirectory, monitorInfo
    data_storage = {}
    workingDirectory = directory
    monitorInfo = monitor_info
    
    # STEPS
    os.chdir(workingDirectory)    
    
    # Loop over all files in the folder
    print("Reading files...")
    for file in os.listdir():
        # check whether file is in text format
        if (file.endswith(".TXT")) or (file.endswith(".txt")):
            txtf, fileD, monitorN = read_log(file, str) # use str as data type for PCPV/exceedance analysis
            if txtf is None:
                continue
            else:
                data_storage = update_data(file, txtf, fileD, monitorN)

    list_of_dates = compile_dates() # Compile a list of all monitoring dates
    freqStrToFloat() # Convert frequencies to floats
    filterPPVwithNaNFreq() # Set PPV values with NaN frequency to also be equal to NaN
    calcPCPV() # Calculate peak component particle velocity and dominant frequency
    exceedances = createExceedanceTable(save_docx)
    daily_maxes = dailyMaxSummary()
    if plot_mode == 'all':
        # Plot data and save in folder "\Figures"
        subPlotMode(list_of_dates, save_plots, max_plot_PPV, save_docx) 
    else:
        print("Normal plot mode not yet implemented. ")
        # normalPlotMode()
    print("Processing complete")

    return exceedances, daily_maxes, data_storage, list_of_dates

    
def read_log(fname, data_type):
    # check if ASCII file is an exceedance report by determining if Line 41 in ASCII file
    # has "Tran" "Vert" "Long"
    print("Processing file: " + fname)
    checkLine41 = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=40, max_rows=1, delimiter='	',dtype=data_type, autostrip=True)
    # skip_rows = adjustBasedOnAdditionalFields(fname)
    if 'Tran' and 'Vert' and 'Long' in checkLine41:
        return None, None, None
    else:
        checkLine16 = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=15, max_rows=1, delimiter='	',dtype=data_type, autostrip=True)
        
        if 'Client' in str(checkLine16):
            dateLine = 50
        else:
            dateLine = 46
        # read ASCII file
        txtData = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=dateLine, delimiter='	',dtype=data_type,autostrip=True)
        # obtain date of file
        fileDate = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=dateLine-1, max_rows=1)
        fileDate = dt.datetime.strptime(fileDate[1][0:-1]+'-'+fileDate[0]+'-'+fileDate[2],'%d-%B-%Y')
        fileDate = fileDate.strftime('%d %B %Y')
        # determine monitor name
        monitorName = str(np.genfromtxt(workingDirectory+"\\"+fname, dtype=data_type, skip_header=1, max_rows=1))
        locB = monitorName.find('B') # locate 'B' to find the start of the minimate name
        # check to see if " is in monitorName
        if '"' in monitorName:
            adjust = -3
        else:
            adjust = -2
        monitorName = monitorName[locB:adjust]
    
        return txtData, fileDate, monitorName

def update_data(fname, txtData, fileDate, monitorName):
# This method creates a nested dictionary and populates it with the monitor names
# in the first layer, and monitoring dates in the second layer
# The monitoring dates layer contains:
    # Array pos 0 is the data from the text file
    # Array pos 1 is where monitoring duration value is stored 
    # Array pos 2 is where daily exceedances are stored
    
    layer1_temp = {monitorName: {}}
    layer2_temp = {fileDate: [txtData,-1,[]]}
    
    # add monitor name to outer dict if it is not there
    if monitorName not in data_storage.keys():
        data_storage.update(layer1_temp)
    
    def _getMonitoringDuration_(fname):
        num_intervals = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=8, max_rows=1)[-1][:-1]
        row_ten = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=9, max_rows=1)
        data_rate = [val for val in row_ten if val.isnumeric()][-1]
        if 'seconds' not in row_ten:
            interval_size = float(data_rate)*60
        elif 'seconds' in row_ten:
            interval_size = float(data_rate)
        else:
            print("getMonitoringDuration() ERROR: interval size is not read correctly in " + fname)
        duration = float(num_intervals)*interval_size
        
        return duration    
    
    # obtain monitoring duration for the day
    duration = _getMonitoringDuration_(fname)
    
    # add date under inner dict if it is not there, otherwise stitch data
    # existing data to existing data and add under relevant date
    # this expects the two files to cover different times within the day 
    if fileDate not in data_storage[monitorName]:
        data_storage[monitorName].update(layer2_temp)
        data_storage[monitorName][fileDate][1] = {'Monitoring duration (seconds)': duration}
    elif fileDate in data_storage[monitorName]: # if a date has multiple ascii files
        data_storage[monitorName][fileDate][0] = np.r_[data_storage[monitorName][fileDate][0],txtData]
        combined_duration = data_storage[monitorName][fileDate][1]['Monitoring duration (seconds)'] + duration
        data_storage[monitorName][fileDate][1] = {'Monitoring duration (seconds)': combined_duration}
        
    return data_storage

def compile_dates():
    temp_list = []
    for monitor in list(data_storage.keys()):
        temp_list.extend(list(data_storage[monitor].keys()))
        
    date_list = []
    # remove duplicates to obtain days in monitoring period
    [date_list.append(dateVal) for dateVal in temp_list if dateVal not in date_list]
    
    return date_list

def getStandard(standard):
    # NOT CURRENTLY USED
    # 1. BS 7385 Line 1 (Reinforced or framed structures, industial and heavy commercial buildings)
    # 2. BS 7385 Line 2 (Unreinforced or light framed structures, residential or light commercial type buildings)
    # 3. DIN 4150-3:2006 short-term vibration Line 1 (Commercial, industrial)
    # 4. DIN 4150-3:2006 short-term vibration Line 2 (Residential)
    # 5. DIN 4150-3:2006 short-term vibration Line 3 (Heritage, sensitive, great intrinsic value)
    if standard == 1:
        standardName = 'BS 7385 Line 1'
    elif standard == 2:
        standardName = 'BS 7385 Line 2'
    elif standard == 3:
        standardName = 'DIN 4150-3:2006 Line 1'
    elif standard == 4:
        standardName = 'DIN 4150-3:2006 Line 2'
    elif standard == 5:
        standardName = 'DIN 4150-3:2006 Line 3'

    return standardName

def damageCriteria(monitorName, dom_freq):
    # standard_name = getStandard(standard)
    standard = monitorInfo[monitorName][3]
    # BS 7385 L1
    if standard == 1:
        if dom_freq >= 4: # Hz
            damage_value = 50 # mm/s
        elif dom_freq < 4:
            damage_value = 0.6 # mm/s
    
    # BS 7385 L2
    elif standard == 2: 
        if dom_freq < 4:
            damage_value = 0.6 # mm/s
        elif ((dom_freq > 4) and (dom_freq) < 15):
            slope = np.log10(20/15)/np.log10(15/4)
            intercept = 20/np.power(15,slope)
            damage_value = intercept*np.power(dom_freq,slope)
        elif ((dom_freq >= 15) and (dom_freq) < 40):
            slope = np.log10(50/20)/np.log10(40/15)
            intercept = 50/np.power(40,slope)
            damage_value = intercept*np.power(dom_freq,slope)
        elif dom_freq >= 40:
            damage_value = 50 # mm/s
    
    # DIN 4150 L1
    elif standard == 3: 
        if ((dom_freq >= 1) and (dom_freq) < 10):
            damage_value = 20 # mm/s
        elif ((dom_freq >= 10) and (dom_freq) < 50):
            damage_value = (40-20)/(50-10)*(dom_freq-10)+20
        elif ((dom_freq >= 50) and (dom_freq) < 100):
            damage_value = (50-40)/(100-50)*(dom_freq-50)+40
        elif dom_freq >= 100:
            damage_value = 50 #mm/s
    
    # DIN 4150 L2
    elif standard == 4:
        if ((dom_freq >= 1) and (dom_freq) < 10):
            damage_value = 5 # mm/s
        elif ((dom_freq >= 10) and (dom_freq) < 50):
            damage_value = (15-5)/(50-10)*(dom_freq-10)+5
        elif ((dom_freq >= 50) and (dom_freq) < 100):
            damage_value = (20-15)/(100-50)*(dom_freq-50)+15
    
    # DIN 4150 L3
    elif standard == 5: 
        if ((dom_freq >= 1) and (dom_freq) < 10):
            damage_value = 3 # mm/s
        elif ((dom_freq >= 10) and (dom_freq) < 50):
            damage_value = (8-3)/(50-10)*(dom_freq-10)+3
        elif ((dom_freq >= 50) and (dom_freq) < 100):
            damage_value = (10-8)/(100-50)*(dom_freq-50)+8
    
    else:
        raise Exception("Standard number specified is invalid. Must be 1 to 5.")

    return damage_value

def freqStrToFloat():
    # Convert frequency values from strings to floats for numeric operations
    # Any frequencies below 4 Hz are ignored
    for monitorName in data_storage.keys():
        for day in data_storage[monitorName]:
            for idx_freq in [2,4,6]:
                freqArray = data_storage[monitorName][day][0][:,idx_freq]
                newFreqArray = []
                for freqVal in freqArray:
                    if freqVal == '<1.0':
                        newFreq = 1.
                    elif freqVal == '>100':
                        newFreq = 100.
                    elif freqVal == 'N/A':
                        newFreq = 100
                    else:
                        newFreq = np.float32(freqVal)
                    # set frequencies below 4 Hz to NaN
                    if newFreq < 4:
                        newFreq = np.nan
                        # possible issue may occur if, for a single data point,
                        # all orthogonal axes have dominant frequencies of less than 4 Hz
                        # which means 'np.nanargmax' will not work
                    newFreqArray = np.append(newFreqArray,newFreq)
                data_storage[monitorName][day][0][:,idx_freq] = newFreqArray
   

def filterPPVwithNaNFreq():
    # Make PPV values corresponding to a NaN frequency also equal to NaN so
    # that they are not used in determining the dominant frequency
    for monitorName in data_storage.keys():
        for day in data_storage[monitorName]:
            for idx_freq in [2,4,6]:
                freqArray = data_storage[monitorName][day][0][:,idx_freq].astype('float32')
                for idx_val in range(0,len(freqArray)):
                    freqVal = freqArray[idx_val]
                    if np.isnan(freqVal):
                        data_storage[monitorName][day][0][idx_val,idx_freq-1] = np.nan

def calcPCPV():
    for monitorName in data_storage.keys():
        for day in data_storage[monitorName]:
            domFreq = []
            temp_PPV = data_storage[monitorName][day][0][:,[1,3,5]].astype(float)
            PCPV = np.nanmax(temp_PPV,axis=1)
            data_storage[monitorName][day][0] = np.c_[data_storage[monitorName][day][0],PCPV]
            idx_PCPV = np.nanargmax(temp_PPV,axis=1)   
            for row in range(0,len(idx_PCPV)):
                PCPV_axis = idx_PCPV[row]
                if PCPV_axis == 0: # PCPV axis is Tran
                    domFreqVal = data_storage[monitorName][day][0][row,2]
                elif PCPV_axis == 1: # PCPV axis is Vert
                    domFreqVal = data_storage[monitorName][day][0][row,4]
                elif PCPV_axis == 2: # PCPV axis is Long
                    domFreqVal = data_storage[monitorName][day][0][row,6]
                    
                domFreq = np.append(domFreq, domFreqVal)
            data_storage[monitorName][day][0] = np.c_[data_storage[monitorName][day][0],domFreq]


    
def subPlotMode(list_of_dates, save_plots, max_plot_PPV, save_docx):
    # convert list of dates to datetime format so they can be sorted
    list_of_days = [dt.datetime.strptime(temp_dt,'%d %B %Y').date() for temp_dt in list_of_dates]
    # convert day to string so the dictionary key can be referenced
    list_of_days = np.sort(list_of_days)
    
    if save_plots:
        # present message if save plot mode is on
        print("Saving figures...")
        
    for day_num, day in enumerate(list_of_days):
        # plt.figure(figNum)
        fig, axs = plt.subplots(len(monitorInfo),1)
        fig.tight_layout()
            
        day = dt.datetime.strftime(day,'%d %B %Y')
        for mn, current_monitor in enumerate(monitorInfo.keys()):
            
            if day in data_storage[current_monitor].keys():
                day_for_monitor_exists = True
            else:
                day_for_monitor_exists = False
            
            if len(monitorInfo.keys()) == 1:
                ax = axs
            elif len(monitorInfo.keys()) > 1:
                ax = axs[mn]

            if day_for_monitor_exists:
                ax.xaxis.axis_date
                ax.grid()
                current_PCPV = data_storage[current_monitor][day][0][:,8].astype(float)
                current_time = data_storage[current_monitor][day][0][:,0]
                current_time = [dt.datetime.strptime(temp_dt,'%H:%M:%S').time() for temp_dt in current_time]
                new_day = dt.datetime.strptime(day,'%d %B %Y').date()
                current_time = [dt.datetime.combine(new_day, temp_time) for temp_time in current_time]
                ax.plot(current_time, current_PCPV, linestyle='-', color='black', label = monitorInfo[current_monitor][2])

                recording_time = data_storage[current_monitor][day][1]['Monitoring duration (seconds)']
                if recording_time < 3600:              
                    ax.xaxis.set_major_locator(mdates.MinuteLocator(interval = 5))
                    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
                if ((recording_time >= 3600) and (recording_time < 14400)):
                    ax.xaxis.set_major_locator(mdates.HourLocator(interval = 1))
                    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
                if recording_time >= 14400:
                    ax.xaxis.set_major_locator(mdates.HourLocator(interval = 2))
                    ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
                    
                # # plot halt and warning levels
                ax.axhline(y=monitorInfo[current_monitor][0],color='orange',linestyle='dashed')
                ax.axhline(y=monitorInfo[current_monitor][1],color='red',linestyle='dashed')           
                # fig.autofmt_xdate() 
                plt.setp(ax.get_xticklabels(),rotation=30)

            else:
                # if no data exists, plot empty axes
                ax.plot([],[],color='black',label = monitorInfo[current_monitor][2])
                ax.xaxis.set_tick_params(labelbottom=False)
                ax.text(0,1,'No data',fontsize=16)
                
                # *************************
                # IMPLEMENT TEXT LABEL THAT SAYS 'NO DATA'                

            ax.set_ylim([0, max_plot_PPV[current_monitor]])
            ax.set_ylabel('PCPV [mm/s]')
            ax.legend()
            if mn == 0:
                new_day = dt.datetime.strptime(day,'%d %B %Y').date()
                weekdayName = new_day.strftime('%A')
                ax.set_title(weekdayName+', '+day,fontsize=10)
            if mn == len(monitorInfo)-1:
                ax.set_xlabel('Time')
            
            # save plots if save_plots is True
            if save_plots:
                isExist = os.path.exists(workingDirectory+'\\Figures')
                # Create directory because it does not exist
                if not isExist:
                    os.makedirs(workingDirectory+'\\Figures')

                fig.savefig(workingDirectory+'\\Figures\\'+day+'.png')

            # # close plots if show_plot is False
            # if ~show_plots:
            #     plt.close()

        if save_docx:
            if day_num == 0:
                doc = docx.Document()                
            doc.add_picture(workingDirectory+'\\Figures\\'+day+'.png', width=Cm(14))
            doc.save(workingDirectory+'\\Figures\\'+'Appendix_Figures.docx')
            
# def normalPlotMode():
#     print("normal plot mode")

#     if save_plots:
#         isExist = os.path.exists(workingDirectory+'\\'+current_monitor)
#         # Create directory because it does not exist
#         if not isExist:
#             os.makedirs(workingDirectory+'\\'+current_monitor)
#         plt.savefig(workingDirectory+'\\'+current_monitor+'\\'+day+'.png')
        
def createExceedanceTable(save_docx):
    # Determine necessary parameters for exceedances
    
    date_of_exceedance = []
    PCPV_exceedance = []
    dom_freq_exceedance = []
    loc_exceedance = []
    damage_PCPV = []
    exceeds_criterion = []
    atleast_one_exceedance = False
    
    for monitorName in data_storage.keys():
        for day in data_storage[monitorName]:
            for ppv_idx in range(0,len(data_storage[monitorName][day][0][:,8])):
                ppv = data_storage[monitorName][day][0][:,8][ppv_idx].astype(float)
                time = data_storage[monitorName][day][0][:,0][ppv_idx]
                
                if ppv > monitorInfo[monitorName][1]:
                    atleast_one_exceedance = True
                    day_temp = dt.datetime.strptime(day,'%d %B %Y')
                    # time_temp = dt.datetime.strptime(time,'%H:%M:%S')
                    dayName = day_temp.strftime('%A')
                    PCPV_exceedance = np.append(PCPV_exceedance,ppv)
                    
                    # Obtain corresponding dominant frequency value to the current ppv value
                    dom_freq_val = data_storage[monitorName][day][0][ppv_idx,9].astype(float)
                    
                    dom_freq_exceedance = np.append(dom_freq_exceedance,dom_freq_val)
                    date_of_exceedance = np.append(date_of_exceedance,dayName+', '+day+' '+time)
                    loc_exceedance = np.append(loc_exceedance,monitorInfo[monitorName][2])
                    
                    damage_val = damageCriteria(monitorName, float(dom_freq_val))
                    damage_PCPV = np.append(damage_PCPV,damage_val)
                    if ppv > damage_val:
                        exceeds_criterion = np.append(exceeds_criterion, 'Yes')
                    elif ppv <= damage_val:
                        exceeds_criterion = np.append(exceeds_criterion, 'No')
    
    # convert to strings so the results can be combined into a single array
    if atleast_one_exceedance:
        loc_exceedance = loc_exceedance.astype(str)
        date_of_exceedance = date_of_exceedance.astype(str)
        PCPV_exceedance = np.round(PCPV_exceedance,2).astype(str)
        dom_freq_exceedance = dom_freq_exceedance.astype(str)
        damage_PCPV = np.round(damage_PCPV,2).astype(str)
        
        exceedance_summary = np.c_[loc_exceedance, date_of_exceedance, PCPV_exceedance, dom_freq_exceedance, damage_PCPV, exceeds_criterion]
        
        exceedance_summary = pd.DataFrame(exceedance_summary,columns=['Location','Exceedance Time','PCPV (mm/s)','Dom Freq','PPV Damage Criterion', 'Exceeds Criterion?'])

    if save_docx:
        doc = docx.Document()
        exceed_table = doc.add_table(rows=1, cols=6)
        
        row = exceed_table.rows[0].cells
        row[0] = 'Location'
        row[1] = 'Date'
        row[2] = 'PCPV (mm/s)'
        row[3] = 'Dominant Frequency (Hz)'
        row[4] = 'Continuous Damage Criterion (mm/s)'
        row[5] = 'Exceeds Criterion?'
        
        
        # for exceed_row in np.shape(exceedance_summary)[0]:
        #     exceedance_summary[]

        return exceedance_summary
    
    else:
        print("No exeedances found")
        return

def dailyMaxSummary():
    daily_max_summary = {}
    for monitorName in data_storage.keys():
        # daily maximum summary dictionary/table
        daily_max_summary.update({monitorName: [['Day','PPV Max','Frequency','Damage Criteria']]})
        for day in data_storage[monitorName]:
            # summary of maximum vibration levels
            max_row = np.argmax(data_storage[monitorName][day][0][:,8].astype(float))
            ppv_max = data_storage[monitorName][day][0][:,8][max_row]
            freq_max = data_storage[monitorName][day][0][:,9][max_row]
            damage_val = damageCriteria(monitorName, float(freq_max))
            
            current_max = np.array([day, str(ppv_max), str(freq_max), str(np.round(damage_val,2))]).reshape(1,4)
            
            daily_max_summary[monitorName] = np.r_[daily_max_summary[monitorName],current_max]  
        
    return daily_max_summary

def quick_plot(fname, workingDirectory):
    
    # DOES NOT WORK YET
    # !!!!!!!!!!!!!!!!!!
    
    
    data_type = float
    # check if ASCII file is an exceedance report by determining if Line 41 in ASCII file
    # has "Tran" "Vert" "Long"
    print("Processing file: " + fname)
    checkLine41 = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=40, max_rows=1, delimiter='	',dtype=data_type, autostrip=True)
    # skip_rows = adjustBasedOnAdditionalFields(fname)
    if 'Tran' and 'Vert' and 'Long' in checkLine41:
        print('File loaded is an exceedance log file')
        return None, None, None
    else:
        checkLine16 = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=15, max_rows=1, delimiter='	',dtype=data_type, autostrip=True)
        
        if 'Client' in str(checkLine16):
            dateLine = 50
        else:
            dateLine = 46
        # read ASCII file
        txtData = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=dateLine, delimiter='	',dtype=float, autostrip=True)
        txtData2 = np.genfromtxt(workingDirectory+"\\"+fname, skip_header=dateLine, delimiter='	', dtype=str, autostrip=True)

        # Determine file date and convert to %d %B %Y format
        fileDate = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=dateLine-1, max_rows=1)
        fileDate = dt.datetime.strptime(fileDate[1][0:-1]+'-'+fileDate[0]+'-'+fileDate[2],'%d-%B-%Y')
        fileDate = fileDate.strftime('%d %B %Y')
        
        # determine recording interval time
        current_time = txtData2[:,0]
        current_time = [dt.datetime.strptime(temp_dt,'%H:%M:%S').time() for temp_dt in current_time]
        new_day = dt.datetime.strptime(fileDate,'%d %B %Y').date()
        current_time = [dt.datetime.combine(new_day, temp_time) for temp_time in current_time]
        
        num_intervals = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=8, max_rows=1)[-1][:-1]
        row_ten = np.genfromtxt(workingDirectory+"\\"+fname, dtype=str, skip_header=9, max_rows=1)
        data_rate = [val for val in row_ten if val.isnumeric()][-1]
        if 'seconds' not in row_ten:
            interval_size = float(data_rate)*60
        elif 'seconds' in row_ten:
            interval_size = float(data_rate)
        else:
            print("getMonitoringDuration() ERROR: interval size is not read correctly in " + fname)
        duration = float(num_intervals)*interval_size

        
        fig, ax = plt.subplots(2,1, sharex='all')
        # ax.set_title('Vibration Velocity')
        ax[0].xaxis.axis_date
        ax[0].plot(current_time, txtData[:,1],'k',label='Tran')
        ax[0].plot(current_time, txtData[:,3],'grey',label='Vert')
        ax[0].plot(current_time, txtData[:,5],'lightsteelblue',label='Long')
        ax[0].set_ylabel('Vibration velocity [mm/s]',fontsize=14)
        ax[0].grid(which='major')
        ax[0].legend(fontsize=12)
        
        ax[1].xaxis.axis_date
        ax[1].plot(current_time, txtData[:,2],linestyle='--',color='k',label='Tran')
        ax[1].plot(current_time, txtData[:,4], linestyle='--',color='grey',label='Vert')
        ax[1].plot(current_time, txtData[:,6], linestyle='--',color='lightsteelblue',label='Long')
        ax[1].set_xlabel('Time [hh:mm]',fontsize=14)
        ax[1].set_ylabel('Dominant Frequency [Hz]',fontsize=14)
        ax[1].grid(which='major')
        ax[1].legend(fontsize=12)
        
        # adjust x-axis date scale depending on monitoring duration
        if duration < 3600:              
            ax[0].xaxis.set_major_locator(mdates.MinuteLocator(interval = 5))
            ax[0].xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
        if ((duration >= 3600) and (duration < 18000)):
            ax[0].xaxis.set_major_locator(mdates.HourLocator(interval = 1))
            ax[0].xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
        if duration >= 18000:
            ax[0].xaxis.set_major_locator(mdates.HourLocator(interval = 2))
            ax[0].xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
        
        return txtData2
 