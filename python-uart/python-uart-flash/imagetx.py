# print image from uart
# 384 dots thermal printer head
# python 3.12
# copyleft 04-12-2024


from serial import Serial
from time import sleep
from serial.tools.list_ports import comports
from PIL import Image
from numpy import array
from os import listdir
import sys


#------------------------- read default config from file ----------------------
fconfig=open('config.txt', 'r')

comstr=''.join(fconfig.readline().rstrip().split('\n'))
baudratestr=''.join(fconfig.readline().rstrip().split('\n'))

fconfig.close()

#-------------------------- check command line arguments ----------------------
a=sys.argv
for i in range(len(a)):
    #print(sys.argv[i])
    if i==1: comstr=sys.argv[i]  #----- replace config ------
    if i==2: baudratestr=sys.argv[i]

#----------------------------- trying open COM port ---------------------------
portfound=False
for i in comports():
    print(i.device)
    if i.device==comstr:
        portfound=True

if portfound==True:
    ser=Serial(comstr, baudrate=int(baudratestr), timeout=1)
    print('Connected to port '+comstr)

    #----------------------- image handling  ------------------------
    imagename=''
    for i in listdir():  #--------- search image in dir ----------
        if len(i.split('.'))==2:
            if i.split('.')[1]=='png':
                imagename=i
                break

    img=Image.open(imagename)

    width,height=img.size  #---- resize image to fit width ----
    newwidth=384
    newheight=int(height*(newwidth/width))
    img=img.resize((newwidth, newheight), Image.Resampling.LANCZOS)

    img=img.convert(mode='1')  #convert image to black & white
    img=array(img)  #convert image to array

    imglen=len(img)  #------ convert image to "C" array ------
    strimglen=int(len(img[0])/8)

    timg=[0]*imglen
    for i in range(imglen):
        strimg=[0]*strimglen
        for k in range(strimglen):
            b=0b10000000
            for offset in range(8):
                if img[i][k*8+offset]==True: strimg[k]|=b
                b>>=1
        timg[i]=strimg
    
    #------------------------------- COM TX -------------------------
    print("TX Start")

    d=len(timg);
    ser.write((d>>8, d&0xff))

    t=d/10
    for i in range(d):
        ser.write(timg[i])
        if i>=t:
            print(f'{(i/d):2.0%}')
            t+=(d/10)
        sleep(0.1)

    ser.close()
    print('TX End')
else:
    print('Error:', comstr, 'not found')


