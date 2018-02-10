PI=192.168.1.10
export PI
ssh -R 8080:127.0.0.1:8080 pi@$PI 


#load files
scp config.tar.gz  autorun.tar.gz webserver.tar.gz pi@$PI:/home/pi


#make a backup
tar -vczf etc-backup.tar /etc/ #upload this file to your host (use scp) ...




echo 'Acquire::http::Proxy "http://127.0.0.1:8080";' > /etc/apt/apt.conf.d/10proxy






#config patches ...

echo denyinterfaces wlan0 >> /etc/dhcpcd.conf

cat ./etc/network/interfaces > /etc/network/interfaces

cat ./etc/hostapd/hostapd.conf > /etc/hostapd/hostapd.conf

# ab hier
cat ./etc/dnsmasq.d/coloroton.conf > /etc/dnsmasq.d/coloroton.conf

cat ./etc/default/hostapd > /etc/default/hostapd

echo "192.168.2.10 coloroton" >> /etc/hosts
echo "192.168.2.10 coloroton.local" >> /etc/hosts




#start hotspot
systemctl restart hostapd.service
systemctl restart dnsmasq.service


#webserver
apt-get install python-flask
