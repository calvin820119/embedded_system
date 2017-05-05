#!/bin/bash
CPUS_DIR='/sys/devices/system/cpu';

case $1 in
	on)
		if [ -z "$2" ]
		then
			echo no value input;
			exit 1;
		fi
		echo 'userspace' >> ${CPUS_DIR}/cpu0/cpufreq/scaling_governor
		echo 'userspace' >> ${CPUS_DIR}/cpu1/cpufreq/scaling_governor
		echo 'userspace' >> ${CPUS_DIR}/cpu2/cpufreq/scaling_governor
		echo 'userspace' >> ${CPUS_DIR}/cpu3/cpufreq/scaling_governor
		echo $2 >> ${CPUS_DIR}/cpu0/cpufreq/scaling_setspeed
		echo $2 >> ${CPUS_DIR}/cpu1/cpufreq/scaling_setspeed
		echo $2 >> ${CPUS_DIR}/cpu2/cpufreq/scaling_setspeed
		echo $2 >> ${CPUS_DIR}/cpu3/cpufreq/scaling_setspeed
		;;

	off)
		echo 'ondemand' >> ${CPUS_DIR}/cpu0/cpufreq/scaling_governor
		echo 'ondemand' >> ${CPUS_DIR}/cpu1/cpufreq/scaling_governor
		echo 'ondemand' >> ${CPUS_DIR}/cpu2/cpufreq/scaling_governor
		echo 'ondemand' >> ${CPUS_DIR}/cpu3/cpufreq/scaling_governor
		;;
	*)
		echo invalid option \"$1\", use on/off as argument.
		exit 1
		;;
esac
exit 0
