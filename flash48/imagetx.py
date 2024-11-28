import serial
from time import sleep
import serial.tools.list_ports as listports
import cv2
import numpy
from PIL import Image

#---------------------------- read config file --------------------------------
fconfig=open('config.txt', 'r')

comstr=''.join(fconfig.readline().rstrip().split('\n'))
baudratestr=''.join(fconfig.readline().rstrip().split('\n'))
imagefilestr=''.join(fconfig.readline().rstrip().split('\n'))

fconfig.close()

#----------------------------- trying open COM port ---------------------------
ports=listports.comports()    

portlist=[]
for i in ports:
    portlist.append(i.device)
    print(i.device)

portfound=False
for i in range(len(ports)):
    if portlist[i]==comstr:
        portfound=True
        break

if portfound==True:
    print('Connected to port '+comstr)
    ser=serial.Serial(comstr, baudrate=int(baudratestr), timeout=1)

    #----------------------- image handling  ------------------------
    img=Image.open(imagefilestr)

    base_width=384  #------- resize image to fit width -------
    wpercent=(base_width/float(img.size[0]))
    hsize=int((float(img.size[1])*float(wpercent)))
    img=img.resize((base_width, hsize), Image.Resampling.LANCZOS)

    r=img.convert('1')
    r.save('temp.png')

    img_path='temp.png'
    img=cv2.imread(img_path, 0)

    imglen=int(len(img))
    simglen=int(len(img[0])/8)

    timg=[0]*imglen
    for y in range(imglen):  
        simg=[0]*simglen
        for i in range(simglen):
            b=[0b10000000]
            for offset in range(8):
                if img[y][i*8+offset]==255: simg[i]|=b[0]
                b[0]>>=1
        timg[y]=simg
    
    #------------------------------- COM TX -------------------------
    print("TX Wait a few minutes")

    d=len(timg);
    data=bytes([d>>8, d&0xff])
    ser.write(data)

    sleep(0.1)

    for i in range(d):
        ser.write(timg[i])
        #print(str(i+1)+' of '+str(d)) ###debug
        sleep(0.2)

    ser.close()
else:
    print('Error: '+comstr+' not found')

