#Spa Soother

##*Description:*

Code and drawing files to create a small box for electronics. The box uses a Rainbowduino and LED matrix to display calming patterns. Also in the box is a Raspberry Pi Zero that can play relaxing sounds of your choice, at the push of a button. The box turns on with a button and shuts off automatically after 20 minutes.

##*Required Software Components:*
In addition to the files in this git -
* Raspbian Jessie
* Music Player Daemon (mpd)
* alsa
* Python
* Arduino 1.6.5-r5 or later

##*In-Depth Description:*
See the post at thisoldgeek.blogspot.com under S2 Spa Soother

##*Configuration:*
Add your own sound files using the blog posting instructions

Download s2_spa_soother.py into /home/pi/scripts/python - service expects this directory.

How To Autorun A Python Script On Boot Using systemd

`sudo nano /lib/systemd/system/spa_soother.service`

#####* Add the following Lines:*

Description=S2 Spa Soother
After=multi-user.target

[Service]
Type=idle
ExecStart=/usr/bin/python /home/pi/scripts/python/s2_spa_soother.py

[Install]
WantedBy=multi-user.target

`sudo chmod 644 /lib/systemd/system/spa_soother.service`

`sudo systemctl daemon-reload`

`sudo systemctl enable spa_soother.service`
 
To check on the service:

`sudo systemctl status spa_soother.service`

##*Notes:*
* This is a difficult project, please read the Overview included in the git before attempting the build!
* SVG drawing does not contain text on buttons or logo

