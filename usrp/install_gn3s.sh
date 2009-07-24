sudo addgroup gn3s
sudo usermod -G gn3s -a $USERNAME
echo 'ACTION=="add", BUS=="usb", SYSFS{idVendor}=="1781", SYSFS{idProduct}=="0b39", GROUP:="gn3s", MODE:="0777"' > tmp1
echo 'ACTION=="add", BUS=="usb", SYSFS{idVendor}=="16c0", SYSFS{idProduct}=="072f", GROUP:="gn3s", MODE:="0777"' > tmp2
cat tmp1 tmp2 > tmpfile
sudo chown root.root tmpfile
sudo mv tmpfile /etc/udev/rules.d/10-gn3s.rules
sudo /etc/init.d/udev stop
sudo /etc/init.d/udev start
ls -lR /dev/bus/usb | grep gn3s