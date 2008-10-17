#/!usr/bin/python

import serial
import commands
import time
import threading

def title():
    helpvar = commands.getoutput('dcop amarok player title')
    if helpvar.find(' - ') > 0:
        pos = helpvar.find(' - ')
        output = ''
        for i in range(pos + 3, len(helpvar)):
            output = output + helpvar[i]
        return output
    else:
        return helpvar
    
    
def artist():
    output = commands.getoutput('dcop amarok player artist')
    if (output!=''):
        return output
    else:
        helpvar = commands.getoutput('dcop amarok player title')
        output = ''
        if helpvar.find(' - ') > 0:
            pos = helpvar.find(' - ')
            for i in range(pos):
                output = output + helpvar[i]
        return output
        
def bit_rate():
    output = commands.getoutput('dcop amarok player bitrate')
    output = output + ' kbps'
    return output

def time_played():
    try:
        seconds = int(commands.getoutput('dcop amarok player trackCurrentTime'))
    except Exception, e:
        return "Fault"
    minutes = seconds / 60
    seconds = seconds % 60
    hours = minutes / 60
    minutes = minutes % 60
    if (seconds < 10):
        seconds = "0%d" % seconds
    else:
        seconds = str(seconds)
    if (minutes < 10):
        minutes = "0%d" % minutes
    else:
        minutes = str(minutes)
    if (hours < 10):
        hours = "0%d" % hours
    else:
        hours = str(hours)
    return "[%s:%s:%s]" % (hours, minutes, seconds)
    
def date_time():
    lt = time.localtime()
    jahr, monat, tag, stunde, minute, sekunde = lt[0:6]
    return '%02i.%02i.%04i  %02i:%02i:%02i' % (tag, monat, jahr, stunde, minute, sekunde)
    
def bearbeiten(inhalt):
    output = ''
    if len(inhalt) > 20:
        for i in range(20):
            output = output + inhalt[i]
        return output
    else:
        for i in range(21-len(inhalt)):
            inhalt = inhalt + ' '
        return inhalt

j = 0


leer = ''        
time_speicher = ''
title_speicher = ''
artist_speicher = ''
zeit_speicher = ''
jspeicher = 0
sekundenspeicher = 0
last = 0

ser = serial.Serial('/dev/rfcomm0', 19200)
time.sleep(3) # 3 sekunden schlaf wegen verbindungsaufbau
ser.write('1' + bearbeiten(leer) + '\n')
ser.write('2' + bearbeiten(leer) + '\n')
ser.write('3' + bearbeiten(leer) + '\n')
ser.write('4' + bearbeiten(leer) + '\n') 
# loeschen der stoerenden CONNECT anzeige auf dem display

while True:
    if (commands.getoutput('dcop amarok player status') != 'call failed' ):
        
        if artist() != artist_speicher:
            ser.write('1' + bearbeiten(artist()) + '\n')
            artist_speicher = artist()
            
        if (title() != title_speicher) or (j != jspeicher):
            if len(title()) > 20:
                hilfe = title()
                output = ''
                jspeicher = j
                if last == 1:
                    for i in range(len(title()) - (j+20), len(title()) - j):
                        try:
                            output = output + hilfe[i]
                        except Exception, e:
                            print e
                    ser.write('2' + bearbeiten(output) + '\n')
                    title_speicher = title()
                else:
                    for i in range(j, j+20):
                        try:
                            output = output + hilfe[i]
                        except Exception, e:
                            print e
                    ser.write('2' + bearbeiten(output) + '\n')
                    title_speicher = title()
            else:
                ser.write('2' + bearbeiten(title()) + '\n')
                title_speicher = title()

        if time_played() != time_speicher:
            ser.write('3' + bearbeiten(time_played() + ' ' + bit_rate()) + '\n')
            time_speicher = time_played()
    else:
        ser.write('1' + bearbeiten('Amarok is not') + '\n')
        ser.write('2' + bearbeiten('playing or an') + '\n')
        ser.write('3' + bearbeiten('Error happened!') + '\n')
    if date_time() != zeit_speicher:
        ser.write('4' + bearbeiten(date_time()) + '\n')
        zeit_speicher = date_time()
        if (commands.getoutput('dcop amarok player status') != 'call failed' ):
            if len(title()) > 20:
                j = j + 1
                if j + 20 > len(title()):
                    j = 0
                    if last == 0:
                        last = 1
                    else:
                        last = 0
            
            















