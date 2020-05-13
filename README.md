# Usage
0. create a DIR **pagecacheFilePath**
1. change **pagecacheFilePath** in **generate_page_cache_file.sh**(line 1), **evict_demo.c**(line 17, 18, 19), and  **load_target_file.sh**(line 1) which currently is "/home/lys/Documents/pagecachefiles"
2. make
3. ./generate_page_cache_file.sh
4. ./load_target_file.sh
5. ./evict_demo
