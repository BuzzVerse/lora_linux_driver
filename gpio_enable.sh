#!/usr/bin/bash

gpio66_direction=/sys/class/gpio/gpio66/direction
gpio66_value=/sys/class/gpio/gpio66/value
gpio69_direction=/sys/class/gpio/gpio69/direction
gpio69_value=/sys/class/gpio/gpio69/value

echo out > $gpio66_direction
echo 1 > $gpio66_value
echo out > $gpio69_direction
echo 1 > $gpio69_value

printf "gpio66: %s %s\n" "$(cat $gpio66_direction)" "$(cat $gpio66_value)"
printf "gpio69: %s %s\n" "$(cat $gpio69_direction)" "$(cat $gpio69_value)"
