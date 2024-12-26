#!/bin/bash
#SBATCH -A pafob1
#SBATCH -p core40q
#SBATCH -n 1

module load gcc/11.2.0
module load intel/vtune_amplifier_xe_2019u4
module load intel/parallel_studio_xe_2019.4.070

chmod +x mcs-par

THREAD_COUNTS=(2 4 8 16)

for THREADS in "${THREAD_COUNTS[@]}"; do
    echo "Running for ${THREADS} threads..."

    export OMP_NUM_THREADS=${THREADS}
    OUTPUT_DIR="output_threads_${THREADS}"
    mkdir -p ${OUTPUT_DIR}

    amplxe-cl -collect hotspots ./mcs-par
    amplxe-cl -report hotspots > ${OUTPUT_DIR}/report_par_hotspots.txt
    
    # Collect threading analysis
    amplxe-cl -collect threading ./mcs-par
    amplxe-cl -report summary > ${OUTPUT_DIR}/report_threading_par_summary.txt
    amplxe-cl -report hw-events >> ${OUTPUT_DIR}/report_threading_par_summary.txt
    
    echo "Results saved in ${OUTPUT_DIR}/"
done
