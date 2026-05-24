#!/bin/bash

echo "=== STL Version ==="
valgrind --tool=massif --massif-out-file=massif_stl.out ./build_stl/lang_it 2>/dev/null
peak_stl=$(ms_print massif_stl.out 2>/dev/null | grep -oP 'peak.*?\d+' | head -1 | grep -oP '\d+')
echo "STL Peak: ${peak_stl} KB"

echo ""
echo "=== ETL Version ==="
valgrind --tool=massif --massif-out-file=massif_etl.out ./build/lang_it 2>/dev/null
peak_etl=$(ms_print massif_etl.out 2>/dev/null | grep -oP 'peak.*?\d+' | head -1 | grep -oP '\d+')
echo "ETL Peak: ${peak_etl} KB"

echo ""
echo "=== Binary Sizes ==="
ls -lh build_stl/lang_it | awk '{print "STL: " $5}'
ls -lh build/lang_it | awk '{print "ETL: " $5}'
