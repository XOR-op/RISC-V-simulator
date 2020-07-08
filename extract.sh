for file in `ls riscv-data/testcases/|grep '\.c'`; do
    echo "${file}"
    cat "riscv-data/testcases/${file}"|grep 'return.*//'|awk '{print $6}'
done
