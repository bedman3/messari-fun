# messari-fun

## Requirement
```text
you need cmake >= version 3.12, linux environment, developed this in Ubuntu 20.04 LTS
gcc, g++ version 9.3.0
```

## Run the program
```bash
git clone git@github.com:bedman3/messari-fun.git
cd messari-fun

cmake .
make

ls -l
# [13:54:25] martin@MartinDesktop:~/test/messari-fun $ ls -l messari
# -rwxr-xr-x 1 martin martin 36664 Mar  3 13:54 messari

# download the messari stdoutinator in https://resonant-zipper-d74.notion.site/Messari-Market-Data-Coding-Challenge-rev-28-Feb-2022-e513357eaeb34b9a9ab9805af37d96b0
./stdoutinator_amd64_linux.bin | ./messari > output.log
```