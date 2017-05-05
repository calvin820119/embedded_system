#!/bin/bash

CPUS_DIR='/sys/devices/system/cpu';

echo [Frequency\(MHz\)]
cat ${CPUS_DIR}/cpu0/cpufreq/cpuinfo_cur_freq
cat ${CPUS_DIR}/cpu1/cpufreq/cpuinfo_cur_freq
cat ${CPUS_DIR}/cpu2/cpufreq/cpuinfo_cur_freq
cat ${CPUS_DIR}/cpu3/cpufreq/cpuinfo_cur_freq

printf "\n"
echo [Governor]
cat ${CPUS_DIR}/cpu0/cpufreq/scaling_governor
cat ${CPUS_DIR}/cpu1/cpufreq/scaling_governor
cat ${CPUS_DIR}/cpu2/cpufreq/scaling_governor
cat ${CPUS_DIR}/cpu3/cpufreq/scaling_governor

exit 0
