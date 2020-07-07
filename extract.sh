for file in `ls riscv-data/|grep '\.c'`; do
    echo "${file}"
    cat "riscv-data/${file}"|grep 'return.*//'|awk '{print $6}'
done
