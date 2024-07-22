#!/usr/bin/bash

gpio66_value=/sys/class/gpio/gpio66/value
gpio69_value=/sys/class/gpio/gpio69/value

echo 0 > $gpio66_value
echo 0 > $gpio69_value
sleep .0001s
echo 1 > $gpio66_value
echo 1 > $gpio69_value
sleep .005s

